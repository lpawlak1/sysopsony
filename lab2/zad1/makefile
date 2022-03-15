lib:
	gcc zad1.c -o zad1 -D LIB
sys:
	gcc zad1.c -o zad1 -D SYS

tests:
	@printf 'LIB\n'
	@make lib
	time ./zad1 ../plik_testowy ../nowy_plik

	@printf 'SYS\n'
	@make sys
	time ./zad1 ../plik_testowy ../nowy_plik

clean:
	rm -rf zad1 ../nowy_plik
