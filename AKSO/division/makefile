
mdiv_example: mdiv.asm mdiv_example.c
	nasm -f elf64 -w+all -w+error -o mdiv.o mdiv.asm
	gcc -c -Wall -Wextra -std=c17 -O2 -o mdiv_example.o mdiv_example.c
	gcc -z noexecstack -o mdiv_example mdiv_example.o mdiv.o
clear:
	rm *.o
	rm mdiv_example
