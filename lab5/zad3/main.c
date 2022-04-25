
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>


int main(int argc, char** argv){

    if(argc != 2){
        exit(12);
    }
    if(mkfifo("pipe", 0666) < 0){
        exit(9);
    }
    int producers_no = atoi(argv[1]);
    for(int i = 0; i < producers_no; i++){
        char file_name[29];
        sprintf(file_name, "producer%dfile.txt", i);
        FILE* f = fopen(file_name, "w");
        char *line = malloc(33* sizeof(char));
        for(int j = 0; j < 30; j++){ 
            line[j] = 'a' + i;
        }
        printf("%s\n", line);
        fprintf(f, "%s", line);
        fclose(f);
        free(line);
    }
    for(int i = 0 ; i < producers_no; i ++){
        if(fork() == 0){
            char buff[10];
            sprintf(buff, "%d", i + 1);
            char file_name[20];
            sprintf(file_name, "prod%df", i);
            execl("./producer", "./producer", "pipe", buff , file_name, "10", NULL);
            printf("co jest...");
            exit(3);
        }
    }
    if(fork() == 0){
        execlp("./consumer", "./consumer", "pipe", "consumer.txt" , "10", NULL);
        exit(3);
    }
    wait(NULL);
    for(int i = 0; i < producers_no; i++) wait(NULL);


    return 0;
}