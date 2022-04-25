
all: compile test clean

compile:
	gcc -Wall main.c -o ab

test:
	./ab nadawca
	@echo ''
	./ab data

clean:
	rm -rf ab
