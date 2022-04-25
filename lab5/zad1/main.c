
// #define _POSIX_C_SOURCE
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_ARGS_NUM 32
#define WRITE_PIPE 1
#define READ_PIPE 0

//skladniki (tablica)
// linia przerwy
// skladniki jak pipe zrobic
//function strstrip
char *strstrip(char *str)
{
    char *end;
    // Trim leading space
    while(isspace(*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str;
}

void exec_commands(char **cmds, size_t task_num){
    int** pipes = malloc(sizeof(int*)*task_num);
    for(int i = 0; i < task_num; i++){pipes[i] = malloc(sizeof(int)*2);pipe(pipes[i]);}

    for(int i = 0; i < task_num; i++){printf("%s\n", strstrip(cmds[i]));}

    for(int i = 0; i < task_num; i++){
        int pid_worker = fork();

        if(pid_worker == 0){
            if(i != 0){ // jak pierwszy to nie ma z czego czytac
                dup2(pipes[i-1][READ_PIPE], STDIN_FILENO);
                close(pipes[i-1][WRITE_PIPE]);
            }
            dup2(pipes[i][WRITE_PIPE], STDOUT_FILENO);
            close(pipes[i][READ_PIPE]);

            int arg_num = 0;
            char *args[MAX_ARGS_NUM];
            memset(args, '\0', MAX_ARGS_NUM * sizeof(char *));
            char *arg = strtok(strstrip(cmds[i]), " ");
            while(arg != NULL){
                args[arg_num++] = strdup(arg);
                arg = strtok(NULL, " ");
            }

            execvp(args[0], &args[0]);
            exit(EXIT_SUCCESS);
        }

        close(pipes[i][WRITE_PIPE]);
        waitpid(pid_worker, NULL, 0);
    }

    char buf[1];
    while(read(pipes[task_num-1][READ_PIPE], &buf[0], sizeof(char)) > 0){
        printf("%c",buf[0]);
    }
    for(int i = 0; i < task_num; i++){free(pipes[i]);}
    free(pipes);
}


int splitbyChar(char* pString, char* splitBy, char** pArray, int* pArraySize){
    int initialSize = 0;
    char* pStart = pString;
    char* pEnd = strstr(pString, splitBy);
    if (pEnd == NULL){
        return 0;
    }
    int i = 0;
    while (pEnd != NULL){
        (pArray)[i] = malloc(sizeof(char) * (pEnd - pStart + 1));
        pEnd[0] = '\0';
        strcpy(pArray[i], pStart);
        i++;
        initialSize++;
        pStart = pEnd + 1;
        pEnd = strstr(pStart, splitBy);
    }
    (pArray)[i] = malloc(sizeof(char) * (strlen(pStart) + 1));
    strcpy(pArray[i], pStart);
    *pArraySize = i + 1;
    return 1;
}
int howManyMate(char* pString, char* splitBy, int* howmany){
    char* pStart = pString;
    char* pEnd = strstr(pString, splitBy);
    if (pEnd == NULL){
        return 0;
    }
    int i = 0;
    while (pEnd != NULL){
        i++;
        pStart = pEnd + 1;
        pEnd = strstr(pStart, splitBy);
    }
    *howmany = i;
    return 1;
}
void read_(char *file_name, char ***lines, int *lines_count) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    *lines_count = 0;
    *lines = malloc(sizeof(char *));
    if (*lines == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, file)) != -1) {
        char *line_copy = malloc(sizeof(char) * (strlen(line) + 1));
        if (line_copy == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(line_copy, line);
        *lines = realloc(*lines, sizeof(char *) * (*lines_count + 1));
        if (*lines == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        (*lines)[*lines_count] = line_copy;
        (*lines_count)++;
    }

    fclose(file);
    if (line) {
        free(line);
    }
}
int checkIfStartWith(char *line, char *start_with){
    return strncmp(line, start_with, strlen(start_with)) == 0;
}
int findIndexOf(char ***lines, int lines_count, char *start_with){
    for (int i = 0; i < lines_count; i++){
        if (checkIfStartWith(lines[i][0], start_with)){
            return i;
        }
    }
    return -1;
}
int main(int argc, char* argv[]) {
    if (argc != 2){
        return 1;
    }
    char *file_name = argv[1];
    char **lines;
    int lines_count;
    read_(file_name, &lines, &lines_count);
    int i = 0;
    for (; i < lines_count; i++){
        if (strcmp(lines[i], "\n") == 0){
            break;
        }
    }
    char*** skladniki = calloc(lines_count, sizeof(char**));
    for (int j = 0; j < i; j++){
        if (checkIfStartWith(lines[j], "skladnik")){
            skladniki[j] = malloc(sizeof(char*) * (2));
            char* pStart = lines[j]; //+ strlen("skladnik");
            char* pEnd = strstr(pStart, "=");
            if (pEnd == NULL){
                return 7;
            }
            pEnd[0] = '\0';
            skladniki[j][0] = calloc(sizeof(char), (strlen(pStart) + 1));
            strcpy(skladniki[j][0], pStart);
//            skladniki[j][0] = strstrip(skladniki[j][0]);

            pStart = ++pEnd;
            skladniki[j][1] = malloc(sizeof(char) * (strlen(pStart) + 1));
            strcpy(skladniki[j][1], pStart);
//            skladniki[j][1] = strstrip(skladniki[j][1]);
        }
    }

    i++;
    for (;i < lines_count; i++ ){
        int howmany = 0;
        howManyMate(lines[i], "|", &howmany);
        char** ret = calloc(howmany+1,sizeof(char*));

        char *legitPolecenie = NULL;

        if (splitbyChar(lines[i], "|", ret, &howmany) == 0) {
            printf("nie ma pipe");
            exit(2);
        }
        else {
            int index = findIndexOf(skladniki, i, strstrip(ret[0]));
            legitPolecenie = malloc(sizeof(char) * (strlen(skladniki[index][1]) + 2));
            sprintf(legitPolecenie, "%s", skladniki[index][1]);
            legitPolecenie[strlen(legitPolecenie)+1] = '\0';


            for (int j = 1; j < howmany; j++) {
                int index = findIndexOf(skladniki, i, strstrip(ret[j]));
                legitPolecenie = realloc(legitPolecenie, sizeof(char) * (strlen(legitPolecenie) + strlen(skladniki[index][1]) + 2));
//                char* buf_idk = calloc(strlen(legitPolecenie)+1, sizeof(char));
                strcat(legitPolecenie, "|");
                strcat(legitPolecenie, skladniki[index][1]);
            }
        }
        for(int l = 0; l < howmany; l++) free(ret[l]);
        free(ret);

        howmany = 0;
        howManyMate(legitPolecenie, "|", &howmany);
        ret = calloc(howmany+1,sizeof(char*));

        if (splitbyChar(legitPolecenie, "|", ret, &howmany) == 0) {
            perror("nie ma pipe");
            exit(9);
        }

        exec_commands(ret, howmany);
        for(int l = 0; l < howmany; l++) free(ret[l]);
        free(ret);
        if(legitPolecenie != NULL)
            free(legitPolecenie);
    }


    for(int i = 0; i< lines_count; i++){free(lines[i]);}
    free(lines);
    for(int l =0; l < lines_count; l++){
        if(skladniki[l] != NULL)
        {free(skladniki[l][0]);
            free(skladniki[l][1]);
        free(skladniki[l]);
        }
    }
    free(skladniki);

    return 0;
}
