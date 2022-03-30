all:
	gcc -Wall -Wno-unused-variable main.c -o main
	gcc -Wall -Wno-unused-variable tester.c -o tester

tests: all
	for lib in ignore handler mask pending ; do \
		for o in fork exec ; do \
			printf "$$lib $$o\n" >> testy.txt ; \
			./main $$o $$lib >> testy.txt ; \
		done ; \
	done

clean:
	rm -rf main tester
