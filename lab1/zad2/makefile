CC=gcc
CFLAGS=-Wall -Wno-unused-variable

static:
	make -C ../zad1 static

clean:
	make -C ../zad1 clean
	rm -f *.o main temp report2.txt

test: static
	$(CC) $(CFLAGS) main.c -I../zad1 -llibrary -L../zad1 -D PERFORMANCE_TESTS -o main
	make report

report:
	rm -f ./report2.txt
	@printf "Static ->\n" 2>/dev/null >> report2.txt
	
	@printf "Jeden plik mały\n" 2>/dev/null >> report2.txt
	./main `cat ./files/arg_1.txt | tr '\n' ' '` 2>/dev/null >> report2.txt
	# #
	@printf "100 slotów, 10 dużych plików\n" >> report2.txt
	./main `cat ./files/arg_2.txt | tr '\n' ' '` 2>/dev/null >> report2.txt
	# #
	@printf "1500 slotów, różne pliki, nie na raz załadowane\n" >> report2.txt
	./main `cat ./files/arg_3.txt | tr '\r\n' ' '` >> report2.txt
	# #
	@printf "1500 slotów, różne pliki, nie na raz załadowane -> caly czas\n" >> report2.txt
	./main `cat ./files/arg_1_time.txt | tr '\r\n' ' '` >> report2.txt
