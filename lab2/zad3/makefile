nftw:
	gcc -o main zad3.c -D NFTW
readdir:
	gcc -o main zad3.c -D READDIR
clean:
	rm -rf ./main
tests:
	@printf 'nftw'
	@make nftw
	@time ./main ./test/

	@printf 'readdir'
	@make readdir
	@time ./main ./test/

	@make clean
