//Zadanie 1 (20%)
//Napisz program, 
//w którym z procesu macierzystego stworzysz dokładnie 
//n procesów potomnych (n parametr wywołania programu). 
//W każdym z procesów potomnych wypisz komunikat, w którym poinformujesz 
//z którego procesu napis pochodzi.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
    //    pid_t fork(void);

void print_smth(int i){
    printf("%d\n", i);
}

int main(int argc, char* argv[]){
    if (argc <= 1){
        return -1;
    }
    int i = 0;
    while (i != atoi(argv[1])){
        int p_id_ = fork();
        if (p_id_ == 0){
            print_smth(i);
            break;
        }
        else
            i++;
    }
}
