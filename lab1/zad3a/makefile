CC=gcc
CFLAGS=-Wall -Wno-unused-variable
LIB_NAME=library
LIB_PATH=../zad1
MAIN_PROG_LOCATION=../zad2
MAIN_PROG_FILE_C=main.c
MAIN_PROG_PATH=$(MAIN_PROG_LOCATION)/$(MAIN_PROG_FILE_C)
REPORT_FILENAME=report3a.txt
REPORT_CMD=2>/dev/null >> $(REPORT_FILENAME) 

static:
	make -C $(LIB_PATH) static
	$(CC) $(CFLAGS) $(MAIN_PROG_PATH) -I$(LIB_PATH) -l$(LIB_NAME) -L$(LIB_PATH) -D PERFORMANCE_TESTS -o main

shared:
	make -C $(LIB_PATH) shared
	$(CC) $(CFLAGS) $(MAIN_PROG_PATH) -I$(LIB_PATH) -l$(LIB_NAME) -L$(LIB_PATH) -D PERFORMANCE_TESTS -o main

dynamic:
	make -C $(LIB_PATH) shared
	$(CC) $(CFLAGS) $(MAIN_PROG_PATH) -ldl -D LIB_DYNAMIC -D PERFORMANCE_TESTS -o main

clean:
	make -C $(LIB_PATH) clean
	make -C $(MAIN_PROG_LOCATION) clean
	rm -f $(REPORT_FILENAME) temp.txt main
	rm -rf files temp
	make -C ../zad1 clean

test: clean
	cp -r $(MAIN_PROG_LOCATION)/files/ ./

	printf "static:\n" $(REPORT_CMD)
	make static
	make report

	printf "shared:\n" $(REPORT_CMD)
	make shared
	make report

	printf "dynamic:\n" $(REPORT_CMD)
	make dynamic
	make report

	rm -rf files temp main

report:
	cp -r $(MAIN_PROG_LOCATION)/files/ ./
	
	@printf "Jeden plik mały\n" >> report3a.txt
	./main `cat ./files/arg_1.txt | tr '\n' ' '` >> report3a.txt
	# #
	@printf "100 slotów, 10 dużych plików\n" >> report3a.txt
	./main `cat ./files/arg_2.txt | tr '\n' ' '` >> report3a.txt
	# #
	@printf "1500 slotów, różne pliki, nie na raz załadowane\n" >> report3a.txt
	./main `cat ./files/arg_3.txt | tr '\r\n' ' '` >> report3a.txt
	# #
	@printf "1500 slotów, różne pliki, nie na raz załadowane -> caly czas\n" >> report3a.txt
	./main `cat ./files/arg_1_time.txt | tr '\r\n' ' '` >> report3a.txt
