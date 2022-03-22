// Zadanie 2 (40%)
// Napisz program, 
// który liczy numerycznie wartość całki oznaczonej z 
// funkcji 4/(x2+1) w przedziale od 0 do 1 
// metodą prostokątów (z definicji całki oznaczonej Riemanna). 
// Pierwszy parametr programu to szerokość każdego prostokąta, 
// określająca dokładność obliczeń. 
// Obliczenia należy rozdzielić na n procesów potomnych 
// (n drugi parametr wywołania programu). 
// Każdy z procesów powinien wynik swojej części obliczeń wpisywać do 
// pliku o nazwie "wN.txt" , 
// gdzie N oznacza numer procesu potomnego liczony od 1 i 
// nadawany procesom w kolejności ich tworzenia. 
// Proces macierzysty powinien oczekiwać na zakończenie wszystkich procesów potomnych 
// po czym powinien dodać wyniki cząstkowe z plików stworzonych przez 
// wszystkie procesy potomne i wyświetlić wynik na standardowym wyjściu.
// W programie zmierz, wypisz na konsolę i zapisz do pliku z raportem czasy realizacji dla różnej liczby procesów potomnych oraz różnych dokładności obliczeń. Dokładności obliczeń należy dobrać w ten sposób by obliczenia trwały co najmniej kilka sekund.

#include <stdlib.h>
#include "malloc.h"
#include "unistd.h"
#include "string.h"
#include <sys/times.h>
#include <stdint.h>
#include <sys/wait.h>

clock_t clock_t_begin, clock_t_end;
struct tms times_start_buffer, times_end_buffer;

void start_timer(){
	clock_t_begin = times(&times_start_buffer);
}

void stop_timer(){
	clock_t_end = times(&times_end_buffer);
}

double calc_time(clock_t s, clock_t e) {
    return ((double) (e - s) / (double) sysconf(_SC_CLK_TCK));
}

void print_times(const char* operation){
	printf("%20s real %.3fs user %.3fs sys %.3fs\n",
        operation,
        calc_time(clock_t_begin, clock_t_end),
        calc_time(times_start_buffer.tms_cutime, times_end_buffer.tms_cutime),
        calc_time(times_start_buffer.tms_cstime, times_end_buffer.tms_cstime));
}

double wartosc(double x){
    return 4/(x*x + 1);
}

double przedzial(double from , double to){
    return wartosc((from+to)/2.0) * (to-from);
}

void print_smth(int i, double wynik){
    char* tabik = calloc(sizeof(char), 17);
    sprintf(tabik, "w%d.txt", i);
    FILE* essa = fopen(tabik, "w+");
    fprintf(essa, "%f", wynik);
    free(tabik);
    fclose(essa);
}

void calc_przedzial(int i, int max, double szerokosc){
    double i_ = (double) i;
    double max_ = (double) max;

    double left = ((i_-1)/max_);
    double right= (i_/max_);

    double wynik = 0.0;

    while (left + szerokosc <= right){
        wynik += przedzial(left,left+szerokosc);
        right -= szerokosc;
    }

    wynik += przedzial(left,right);

    print_smth(i, wynik);
}

int main(int argc, char* argv[]){
    if (argc <= 2){
        return -1;
    }

    start_timer();

    int i = 1;
    int index = 0 ;
    int max = atoi(argv[2]);
    int* p_ids = calloc(sizeof(int), max);
    int p_id_ = 0;
    while (i <= max){
        p_id_ = fork();
        p_ids[index] = p_id_;
        index++;
        if (p_id_ == 0){ // child
            calc_przedzial(i, max, atof(argv[1]));
            break;
        }
        else
            i++;
    }
    if (p_id_ != 0){ // parent
        pid_t wpid;
        int status = 0;
        for(int i = 0; i < index; i++){
            while ((wpid = wait(&status)) > 0);
        }
        //w teorii sie wszystko skonczylo
        double wynik = 0;
        for (int i = 1; i <= max; i++){
            char* essa = calloc(sizeof(char), 10);
            sprintf(essa, "w%d.txt", i);

            FILE* file_ = fopen(essa, "r+");

            fseek(file_, 0, SEEK_END);
            long fsize = ftell(file_);
            fseek(file_, 0, SEEK_SET);  /* same as rewind(f); */

            char *string = malloc(fsize + 1);
            fread(string, fsize, 1, file_);

            string[fsize] = 0;
            double wynik_double_inside = atof(string);
            wynik += wynik_double_inside;

            fclose(file_);
            free(essa);
        }

        stop_timer();

        printf("wynik: %f\nszerokosc prostokata: %f\n", wynik, atof(argv[1]));
        char * message = calloc(sizeof(char), 20);
        sprintf(message, "ilosc wątków: %d\n", max);
        print_times(message);
        printf("\n");
        free(message);
    }
    return 0;
}