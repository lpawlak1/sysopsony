// Zadanie 1 (30%)
#define __USE_POSIX199309
// #define _POSIX_C_SOURCE
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>


//ignore = 0
//handler = 1
//mask = 3
//pending = 4

int waited_for_long = 0;
int odebrane = 0;
pid_t sender_id= 0;

#define jakies_synagly_real_time
#ifdef jakies_synagly_real_time
    #undef SIGUSR1
    #define SIGUSR1 SIGRTMIN+1
    #undef SIGUSR2
    #define SIGUSR2 SIGRTMIN+2
    #define kill_
#endif


void sigusr1_handler(int signo, siginfo_t *info, void *context){
	if(signo != SIGUSR1){ // tylko kill SIGUSR1 nas interesuje
		return; 
	}

	if(info->si_code == SI_USER){
        odebrane++;
        printf("SIGUSR1 received from child:)\n");
	}
}

void sigusr2_handler(int signo, siginfo_t *info, void *context){
	if(signo != SIGUSR2){ // tylko kill SIGUSR2 nas interesuje
		return; 
	}

	if(info->si_code == SI_USER){
        // odebrane++;
        printf("SIGUSR2 (in parent) received\n");
        sender_id = info->si_pid;
        waited_for_long = 1;
	}
}

int main(int argc, char** argv) {

	struct sigaction action;
	action.sa_sigaction = sigusr1_handler; // instaluje handler
	action.sa_flags = SA_SIGINFO;
	sigemptyset(&action.sa_mask);

	if(sigaction(SIGUSR1, &action, NULL) == -1){
		printf("Cant catch SIGUSR1\n");
		return -1;
	}

	struct sigaction action2;
	action2.sa_sigaction = sigusr2_handler; // instaluje handler
	action2.sa_flags = SA_SIGINFO;
	sigemptyset(&action2.sa_mask);

	if(sigaction(SIGUSR2, &action2, NULL) == -1){
		printf("Cant catch SIGUSR2\n");
		return -1;
	}

    printf("My PID: %d\n", getpid());

    sender_id = atoi(argv[1]);
    int do_wyslanie = atoi(argv[2]);

    #ifdef kill_

    {
        for (int i = 0; i < do_wyslanie; i++){
            kill(sender_id, SIGUSR1);
        }
        kill(sender_id, SIGUSR2);
    }

    #endif

    // #define sigqueue_
    #ifdef sigqueue_
    {
        union sigval value;
        for (int i = 0; i < do_wyslanie; i++){
            union sigval value;
            value.sival_int = i;

            // sigqueue(sender_id, SIGUSR1, value);
            if(sigqueue(sender_id, SIGUSR1, value) == 0) {
                printf("signal sent successfully!!\n");
            } else {
                perror("SIGSENT-ERROR:");
            }
        }
        sigqueue(sender_id, SIGUSR2, value);
    }
    #endif
    

    sigset_t maska;
    sigemptyset(&maska);
    sigaddset(&maska, SIGUSR2); // czekam na 2, bo po co czekac na 1
    sigsuspend(&maska); // czekam na SIGUSR2


    printf("wyslano: %d\nodebrano: %d\n", do_wyslanie, odebrane);
    

	return 0;
}
