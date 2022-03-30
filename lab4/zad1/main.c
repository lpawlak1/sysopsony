
// Zadanie 1 (30%)
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>


//ignore = 0
//handler = 1
//mask = 3
//pending = 4


int get_mode_from_arg(const char *arg){
	if(strcmp(arg, "ignore") == 0){
		return 0;
	}else if(strcmp(arg, "handler") == 0){
		return 1;
	}else if(strcmp(arg, "mask") == 0){
		return 3;
	}else if(strcmp(arg, "pending") == 0){
		return 4;
	}else{
		exit(1);
	}
}

void sigusr1_handle(int sigint) {
    printf("SIGUSR1 received\n");
}

int main(int argc, char** argv) {
	if(argc != 3){
		// printf("Usage: main [ignore|handler|mask|pending] [fork|exec]\n");
		return 1;
	}
	int mode = get_mode_from_arg(argv[2]);

	switch(mode){
		case 0:
			signal(SIGUSR1, SIG_IGN);
			raise(SIGUSR1);	//this should be ignored
			break;

		case 1:
			signal(SIGUSR1, sigusr1_handle);
			raise(SIGUSR1);
			break;

		case 3:
		case 4:
			;sigset_t block_mask; // XDDDDDDDDDDDDDDDDD ten srednik jest op
			sigemptyset(&block_mask);
			sigaddset(&block_mask, SIGUSR1);
			sigprocmask(SIG_BLOCK, &block_mask, NULL);
			raise(SIGUSR1); //this should be ignored (added to signal queue)
			if(mode == 4){
				sigset_t current_signals;
				sigpending(&current_signals);
				printf("SIGUSR1 pending in tha parent: %s\n", sigismember(&current_signals, SIGUSR1) ? "yes" : "no");
			}
			break;
		default:
			break;
	}

	if(strcmp(argv[1], "fork") == 0){
		pid_t child = fork();
		if(child == 0){
			switch(mode){
				case 1:
					raise(SIGUSR1);
					printf("SIGUSR1 ignored in child\n");
					break;
				
				case 2:
					raise(SIGUSR1);
					break;

				case 3:
					;raise(SIGUSR1);
					printf("SIGUSR1 masked in child\n");
					break;

				case 4:
					;sigset_t current_signals; // XD te sredniki z 2 stron to są niezłe
					sigpending(&current_signals);
					printf("SIGUSR1 pending in tha child: %s\n", sigismember(&current_signals, SIGUSR1) ? "yes" : "no");
			}
		}else if(child > 0){
			wait(0);
		}

	}else if(strcmp(argv[1], "exec") == 0){
		execl("./tester", "./tester", argv[2], NULL);
	}else{
		return 1;
	}

	return 0;
}
