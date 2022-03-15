#include <stdio.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    char znak;
    char buffer[256];

    scanf("%c", &znak);
    scanf("%s", buffer);

    // ile razy jest podany znak
    // liczba wierszy
    // linie konczy \n
    
    #ifdef LIB
        FILE* file = fopen(buffer, "r+");
        char* buff = calloc(1, sizeof(char));

        unsigned long ilosc_lini = 0, ilosc_znakow = 0;
        short is_found_here = 0;
        while (fread(buff, sizeof(char), 1, file) == 1){
            if (*buff == '\n'){
                if (is_found_here != 0){
                    ilosc_lini++;
                }
                is_found_here=0;
            }
            else if (*buff == znak){
                ilosc_znakow++;
                is_found_here++;
            }
        }
        printf("%ld, %ld\n", ilosc_lini, ilosc_znakow);
        free(buff);
        fclose(file);
    #endif

    #ifdef SYS
        int file = open(buffer, O_RDONLY);
        char* buff = calloc(1, sizeof(char));

        unsigned long ilosc_lini = 0, ilosc_znakow = 0;
        short is_found_here = 0;
        while (read(file, buff, sizeof(char)) == 1){
            if (*buff == '\n'){
                if (is_found_here != 0){
                    ilosc_lini++;
                }
                is_found_here=0;
            }
            else if (*buff == znak){
                ilosc_znakow++;
                is_found_here++;
            }
        }
        printf("%ld, %ld\n", ilosc_lini, ilosc_znakow);
        free(buff);
        close(file);
    #endif
}
