#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


//ignore = 0
//handler = 1
//mask = 3
//pending = 4

int get_mode_from_arg(const char *arg){
	if(strcmp(arg, "ignore") == 0){
		return 0;
	}else if(strcmp(arg, "mask") == 0){
		return 3;
	}else if(strcmp(arg, "pending") == 0){
		return 4;
	}else{
		printf("Bez przesady że handler exec poleciał, tak nie miało być...\n");
		exit(1);
	}
}

int main(int argc, char** argv) {
	int  mode = get_mode_from_arg(argv[1]);

	switch(mode){
		case 0:
			raise(SIGUSR1); //this should be ignored
			printf("SIGUSR1 ignored in child\n");
			break;
		
		case 3:
			raise(SIGUSR1); //this should be ignored
			printf("SIGUSR1 masked in child\n");
			break;

		case 4:
			;sigset_t current_signals; 
			sigpending(&current_signals);
			printf("SIGUSR1 pending in child: %s\n", sigismember(&current_signals, SIGUSR1) ? "yes" : "no");
	}
	return 0;
}
