#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>


#ifdef LIB
#include <stdlib.h>
/**
 * check if current line consists of only white chars
 *
 * takes file
 *
 * returns -1 if it's end of file
 *         -2 if line consists only of white chars
 *         size of line if line consists not only of white chars
 */
int check_if_white_line(FILE* file){
    char* buff = calloc(1, sizeof(char));
    int current_place = ftell(file);
    int ret = 0;
    int i = 0;
    while (fread(buff, sizeof(char), 1, file) == 1 && *buff != '\n'){
        i++;
        if (!isspace(*buff)){
            ret++;
        }
    }
    if (fread(buff, sizeof(char), 1, file) == 0 && ret == 0){
        return -1;
    }
    if (ret != 0){
        fseek(file, current_place, SEEK_SET);
    }
    if (ret == 0){
        fseek(file, -1, SEEK_CUR);
        return -2;
    }
    return i;
}
#endif

#ifdef SYS

#include <fcntl.h>
#include <unistd.h>

/** check if current line consists of only white chars
 *
 * takes file
 *
 * returns -1 if it's end of file
 *         -2 if line consists only of white chars
 *         size of line if line consists not only of white chars
 */
int check_if_white_line(int file){
    char* buff = calloc(1, sizeof(char));
    
    int current_place = lseek(file, 0L, SEEK_CUR);
    
    int ret = 0;
    int i = 0;
    
    while (read(file, buff, sizeof(char)) == 1 && *buff != '\n'){
        i++;
        if (!isspace(*buff)){
            ret++;
        }
    }
    if (read(file, buff, sizeof(char)) == 0 && ret == 0){
        free(buff);
        return -1;
    }
    free(buff);
    if (ret != 0){
        lseek(file, current_place, SEEK_SET);
    }
    if (ret == 0){
        lseek(file, -1, SEEK_CUR);
        return -2;
    }
    return i;
}
#endif

int main(int argc, char* argv[]){
    if (argc <= 1){
        fprintf(stderr, "Not enough arguments");
    }

    char* input[2];
    if (argc == 3){
        input[0] = argv[1];
        input[1] = argv[2];
    }
    else{
        char* buff = calloc(sizeof(char), 500);
        scanf("%s", buff);
        input[0] = calloc(strlen(buff),sizeof(char));

        free(buff);
        
        buff = calloc(sizeof(char), 500);
        scanf("%s", buff);
        input[1] = calloc(strlen(buff), sizeof(char));
        free(buff);
    }

    #ifdef LIB
        FILE* file_write = fopen(input[1], "w+");
        FILE* file = fopen(input[0], "r+");
       
        for (int essa = check_if_white_line(file);essa != -1; essa = check_if_white_line(file)){
            if (essa == -2){ // only white chars
                continue;
            }
            essa++;
            char* tab = calloc(essa, sizeof(char));
            fread(tab, sizeof(char), essa, file);

            fwrite(tab, sizeof(char), essa, file_write);
            free(tab);
        }
        fclose(file);
        fclose(file_write);
    #endif

    #ifdef SYS
        int file_write = open(input[1], O_CREAT | O_WRONLY);
        int file = open(input[0], O_CREAT | O_RDONLY);
        
        for (int essa = check_if_white_line(file);essa != -1; essa = check_if_white_line(file)){
            if (essa == -2){ // only white chars
                continue;
            }
            essa++;
            char* tab = calloc(essa, sizeof(char));
            read(file, tab, sizeof(char)*essa);
            
            write(file_write, tab, sizeof(char)*essa);
            free(tab);
        }
        close(file);
        close(file_write);
    #endif
   
}
