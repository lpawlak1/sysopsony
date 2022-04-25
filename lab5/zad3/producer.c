#define __USE_POSIX199309
// #define _POSIX_C_SOURCE
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <ctype.h>
#include "time.h"

int main(int argc, char** argv){
    srand(time(NULL));
    if(argc != 5){
        printf("Wrong number of arguments");
        return -1;
    }
    char* path_pipe = argv[1];
    char* row_no = argv[2];
    char* path_file = argv[3];
    int N = atoi(argv[4]);

    printf("Producers pid: %d\n", getpid());


    FILE *file = fopen(path_file, "r");
    FILE *fd_file = fopen(path_pipe, "w");


    if(file == NULL || fd_file == NULL){
        printf("File opening failure");
        return -1;  
    }

    char buff[N+1];
    while(fgets(buff, N+1, file) != NULL){ 
        fprintf(fd_file, "%s #%s# ", buff, row_no);
    }
    fclose(file);
    if(fclose(fd_file) == -1){
        printf("Pipe closing failure");
        return -1;
    }
    return 0;
}