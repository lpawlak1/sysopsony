
all: 
	gcc -Wall zad2.c
	make clean

compile:
	gcc -Wall zad2.c


clean:
	rm a.out 
	# niestety nie da sie latwo usunac wN.txt albo ja nie wiem jak, lukasz



test: compile
	echo '' > report_1.md
	./a.out 0.000000001 16 >> report_1.md
	./a.out 0.000000001 12 >> report_1.md
	./a.out 0.000000001 13 >> report_1.md
	./a.out 0.000000001 14 >> report_1.md
	./a.out 0.000000001 15 >> report_1.md
	./a.out 0.000000001 11 >> report_1.md
	./a.out 0.000000001 10 >> report_1.md
	./a.out 0.0000001 9 >> report_1.md
	./a.out 0.000000001 9 >> report_1.md
	./a.out 0.0000001 9 >> report_1.md
	./a.out 0.000000001 9 >> report_1.md
	./a.out 0.000000001 9 >> report_1.md
	./a.out 0.000001 9 >> report_1.md
	./a.out 0.000000001 9 >> report_1.md
	./a.out 0.0001 9 >> report_1.md
	./a.out 0.000000001 9 >> report_1.md
	./a.out 0.0001 9 >> report_1.md
	make clean
	mv report_1.md report_1.txt