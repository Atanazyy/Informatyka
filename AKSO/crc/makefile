crc: crc.asm
	nasm -f elf64 -w+all -w+error -o crc.o crc.asm
	ld --fatal-warnings -o crc crc.o

clear:
	rm crc.o
	rm crc
	rm test.out