CFLAGS=-Wall -Werror

.PHONY: clean run-main run-interactive

run-main: main
	./main

run-interactive: interactive
	./interactive

interactive: interactive.c imageprocessing.c bmp.c
	gcc -ggdb3 interactive.c imageprocessing.c bmp.c $(CFLAGS) -o interactive

main: main.c imageprocessing.c bmp.c
	gcc -ggdb3 main.c imageprocessing.c bmp.c $(CFLAGS) -o main

clean:
	rm -f main interactive
