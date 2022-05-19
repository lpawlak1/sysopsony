
compile:
	gcc -o main main.c -lm -pthread

test: compile
	./main 1 numbers ./dragon.ascii.pgm ./nwm.out.ascii.pgm
	./main 2 numbers ./dragon.ascii.pgm ./nwm.out.ascii.pgm
	./main 4 numbers ./dragon.ascii.pgm ./nwm.out.ascii.pgm
	./main 8 numbers ./dragon.ascii.pgm ./nwm.out.ascii.pgm
	
	./main 1 blocks ./dragon.ascii.pgm ./nwm.out.ascii.pgm
	./main 2 blocks ./dragon.ascii.pgm ./nwm.out.ascii.pgm
	./main 4 blocks ./dragon.ascii.pgm ./nwm.out.ascii.pgm
	./main 8 blocks ./dragon.ascii.pgm ./nwm.out.ascii.pgm
