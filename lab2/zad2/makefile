lib:
	gcc zad2.c -o zad2 -D LIB
sys:
	gcc zad2.c -o zad2 -D SYS

tests:
	@printf 'LIB\n'
	@make lib
	time ./zad2 < args

	@printf 'SYS\n'
	@make sys
	time ./zad2 < args

clean:
	rm -rf zad2