CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=gnu17 -fPIC -O2
LDFLAGS = -shared -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup

.PHONY: all clean
	
libnand.so: nand.o list.o memory_tests.o
	gcc $(LDFLAGS) -o $@ $^
memory_tests.o: memory_tests.c memory_tests.h
	gcc -c $(CFLAGS) -o $@ $<
nand.o: nand.c list.h nand.h
	gcc -c $(CFLAGS) -o $@ $<
list.o: list.c nand.h list.h
	gcc -c $(CFLAGS) -o $@ $<
	
nand_example.o: nand_example.c nand.h memory_tests.h
	gcc $(CFLAGS) -c -o $@ nand_example.c
nand_example: nand_example.o libnand.so
	gcc $(CFLAGS) -L. -o $@ $< -lnand

clean:
	rm -f nand.o
	rm -f list.o
	rm -f memory_tests.o
	rm -f libnand.so
	rm -f nand_example.o
	rm -f nand_example
