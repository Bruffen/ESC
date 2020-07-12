all: naive naive2

naive: naive.c
	gcc -O1 -std=c99 -ggdb -march=native naive.c -o naive -lm

naive2: naive2.c
	gcc -O1 -std=c99 -ggdb -march=native naive2.c -o naive2 -lm

clean:
	rm -f *.o .a *~ Makefile.bak naive

clean2:
	rm -f *.o .a *~ Makefile.bak naive2