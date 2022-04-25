#define __USE_POSIX199309
#define __USE_POSIX200809
// #define _POSIX_C_SOURCE
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char* argv[]) {

    if (argc == 2){ // tylko nadawca / data
        if (strcmp(argv[1], "nadawca") == 0) {
            FILE* fd = popen("ls -al --full-time | awk '{print $6 $7,$9}' | sort -k2 | awk '{print $2}'", "r");
            char buf[1];
            long bytes;
            while ((bytes=fread(buf, sizeof(char), 1, fd))>0)
                fwrite(buf, bytes, 1, stdout);
            pclose(fd);
        }
        else if (strcmp(argv[1], "data") == 0){
            FILE* fd = popen("ls -al --full-time | awk '{print $6 $7,$9}' | sort -k1 | awk '{print $2}'", "r");
            char buf[1];
            long bytes;
            while ((bytes=fread(buf, sizeof(char), 1, fd))>0)
                fwrite(buf, bytes, 1, stdout);
            pclose(fd);
        } else{
            perror("zly argument");
            exit(7);
        }
    }
    if (argc == 4) { // adres tytul tresc
        char command[] = "cat";
        FILE* fd = popen(command, "w");
        fputs(argv[3], fd);
        pclose(fd);
    }
}
