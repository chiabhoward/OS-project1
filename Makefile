CFLAG = -DDEBUG -Wall -std=c99

main: main.o scheduler.o tool.o
	gcc $(CFLAG) main.o scheduler.o tool.o -o main.out

main.o: main.c Makefile
	gcc $(CFLAG) main.c -c
scheduler.o: scheduler.c scheduler.h Makefile
	gcc $(CFLAG) scheduler.c -c
tool.o: tool.c tool.h Makefile
	gcc $(CFLAG) tool.c -c
clean:
	rm -rf *o