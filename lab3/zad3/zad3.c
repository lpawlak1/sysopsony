#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>
#include "malloc.h"
#include "unistd.h"
#include "string.h"
#include <sys/times.h>
#include <stdint.h>
#include <sys/wait.h>


int zwykle_pliki = 0;

char* better_ctime(const __time_t* essa){ // without sigsegv i guess
    char *buff = calloc(sizeof(char), 20);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(essa));
    return buff;
}

int checkIfContains(char* path, char* string_to_search, char* basePath){


    FILE* file_ = fopen(path, "r+");
    if (file_ == NULL){
        return -1;
    }
    fseek(file_, 0, SEEK_END);
    long fsize = ftell(file_);
    fseek(file_, 0, SEEK_SET);  /* same as rewind(f); */

    char* buff = calloc(sizeof(char), fsize);
    fread(buff,sizeof(char), fsize, file_);
    buff[fsize] = 0;

    int ret = 0;
    if (strstr(buff, string_to_search) != NULL){
        printf("PID: %d, path: %s\n", getpid(), basePath);
    }

    fclose(file_);
    free(buff);

    return ret;
}

/*
* najpierw sobie przejde po katalogu, sprawdze ile jest tam katalogów
* za drugim razem sobei zrobie tablice na pid i zaczne dzialac jak trzeba
*/
void myfilerecursive(char *basePath, char* string_to_search, int max_recur, int curr_rec)
{
    char* path = calloc(sizeof(char), 1000);
    int katalogi = 0;
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir){
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            int sg = dp->d_type;
            if (DT_DIR == sg){
                katalogi++;
            }
            if (DT_REG == sg){
                sprintf(path, "%s/%s", basePath, dp->d_name);

                char* result = realpath(path, NULL);

                checkIfContains(result, string_to_search, path);
                free(result);
            }

        }
    }

    free(path);


    closedir(dir);

    if (curr_rec >= max_recur) return;

    pid_t* childs = calloc(sizeof(pid_t), katalogi);
    int index = 0;

    dir = opendir(basePath);
    if (!dir) return;

    pid_t child = 0;
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type == DT_DIR){
            //create smth
            child = fork();
            if (child != 0){ // parent
                childs[index] = child;
            }
            else{ //child
                char* path2 = calloc(sizeof(char), strlen(dp->d_name)+2 + strlen(basePath));
                sprintf(path2, "%s/%s", basePath, dp->d_name);

                myfilerecursive(path2, string_to_search, max_recur, curr_rec+1);
                free(path2);
                break;
            }

        }
    }
    if (child != 0){ // parent, should wait for all
        closedir(dir);
        pid_t wpid;
        int status = 0;
        for(int i = 0; i < index; i++){
            while ((wpid = wait(&status)) > 0);
        }
    }
}

int main(int argc, char* argv[]){
    if (argc <= 3){
        return 1;
    }
    //1 to sciezka
    //2 to co ma byc wyszukane
    //3 to jaki max recur
    myfilerecursive(argv[1], argv[2], atoi(argv[3]), 0);

}
