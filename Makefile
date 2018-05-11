all:
	gcc main.c -o bin
san:
	gcc -fsanitize=address main.c -o bin
ch1:
	./checkpatch.pl -no-tree -f main.c
ch2:
	cppcheck --enable=all --inconclusive --std=posix main.c
