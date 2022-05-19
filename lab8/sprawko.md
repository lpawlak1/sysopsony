gcc -o main main.c -lm -pthread
./main 1 numbers ./dragon.ascii.pgm ./nwm.out.ascii.pgm
0
0 k
1 t done
 Real time: 1380 milisecs
 User time: 1043 milisecs
 System time: 348 milisecs
./main 1
 
./main 2 numbers ./dragon.ascii.pgm ./nwm.out.ascii.pgm
1
0
1 k
1 t done
0 k
2 t done
 Real time: 906 milisecs
 User time: 1160 milisecs
 System time: 387 milisecs
./main 2
 
./main 4 numbers ./dragon.ascii.pgm ./nwm.out.ascii.pgm
1
0
3
2
1 k
1 t done
0 k
2 t done
3 k
3 t done
2 k
4 t done
 Real time: 754 milisecs
 User time: 1569 milisecs
 System time: 523 milisecs
./main 4
 
./main 8 numbers ./dragon.ascii.pgm ./nwm.out.ascii.pgm
5
1
2
4
3
1 k
2 t done
0
7
6
2 k
3 t done
4 k
4 t done
3 k
5 t done
0 k
6 t done
6 k
8 t done
7 k
8 t done
5 k
8 t done
 Real time: 855 milisecs
 User time: 2352 milisecs
 System time: 784 milisecs
./main 8
 
./main 1 blocks ./dragon.ascii.pgm ./nwm.out.ascii.pgm
0
0 k
1 t done
 Real time: 1637 milisecs
 User time: 1645 milisecs
 System time: 0 milisecs
./main 1
 
./main 2 blocks ./dragon.ascii.pgm ./nwm.out.ascii.pgm
0
1
1 k
1 t done
0 k
2 t done
 Real time: 1508 milisecs
 User time: 2866 milisecs
 System time: 0 milisecs
./main 2
 
./main 4 blocks ./dragon.ascii.pgm ./nwm.out.ascii.pgm
0
2
1
3
1 k
1 t done
2 k
1 t done
0 k
2 t done
3 k
3 t done
 Real time: 2463 milisecs
 User time: 5950 milisecs
 System time: 1984 milisecs
./main 4
 
./main 8 blocks ./dragon.ascii.pgm ./nwm.out.ascii.pgm
0
3
2
5
4
7
1
6
1 k
2 t done
6 k
2 t done
5 k
3 t done
4 k
4 t done
2 k
5 t done
3 k
0 k
7 t done
7 t done
7 k
8 t done
 Real time: 3410 milisecs
 User time: 10194 milisecs
 System time: 4415 milisecs
./main 8



Jak jest każdy widzi, wiecej czasu zajmuje 8 wątków niż 4 bo to za dużo na tego dragona z testów + overhead z przehodzeniem wszędzie.
Block jest wolnieszy niż numbers, tak z definicji. 
Koniec zawiłego, pełnego zwrotów akcji, sprawozdania :)
