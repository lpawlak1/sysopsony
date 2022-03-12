CC=gcc
CFLAGS=-Wall -Wno-unused-variable
LIB_NAME=library
LIB_PATH=../zad1
MAIN_PROG_LOCATION=../zad2
MAIN_PROG_FILE_C=main.c
MAIN_PROG_PATH=$(MAIN_PROG_LOCATION)/$(MAIN_PROG_FILE_C)
REPORT_FILENAME=report3b.txt
REPORT_CMD=2>/dev/null >> $(REPORT_FILENAME)

static:
	make -C $(LIB_PATH) static
	$(CC) $(CFLAGS) $(OPTIMIZATION) $(MAIN_PROG_PATH) -I$(LIB_PATH) -l$(LIB_NAME) -L$(LIB_PATH) -D PERFORMANCE_TESTS -o main

shared:
	make -C $(LIB_PATH) shared
	$(CC) $(CFLAGS) $(OPTIMIZATION) $(MAIN_PROG_PATH) -I$(LIB_PATH) -l$(LIB_NAME) -L$(LIB_PATH) -D PERFORMANCE_TESTS -o main

dynamic:
	make -C $(LIB_PATH) shared
	$(CC) $(CFLAGS) $(OPTIMIZATION) $(MAIN_PROG_PATH) -ldl -D LIB_DYNAMIC -D PERFORMANCE_TESTS -o main

clean:
	make -C $(LIB_PATH) clean
	make -C $(MAIN_PROG_LOCATION) clean
	rm -f $(REPORT_FILENAME) temp.txt main
	rm -rf files temp report3b.txt
	make -C ../zad1 clean

test: clean
	cp -r $(MAIN_PROG_LOCATION)/files/ ./

	for lib in static shared dynamic ; do \
		for o in O0 O1 O2 O3 Ofast Os ; do \
			make $$lib OPTIMIZATION=-$$o; \
			printf "$$lib $$o\n" $(REPORT_CMD) ; \
			make report ; \
		done ; \
	done

	rm -rf files

report:
	cp -r $(MAIN_PROG_LOCATION)/files/ ./

	@printf "Jeden plik mały\n" >> report3b.txt
	./main `cat ./files/arg_1.txt | tr '\n' ' '` >> report3b.txt
	# #
	@printf "100 slotów, 10 dużych plików\n" >> report3b.txt
	./main `cat ./files/arg_2.txt | tr '\n' ' '` >> report3b.txt
	# #
	@printf "1500 slotów, różne pliki, nie na raz załadowane\n" >> report3b.txt
	./main `cat ./files/arg_3.txt | tr '\n' ' '` >> report3b.txt
	# #
	@printf "1500 slotów, różne pliki, nie na raz załadowane -> caly czas\n" >> report3b.txt
	./main `cat ./files/arg_1_time.txt | tr '\n' ' '` >> report3b.txt
