
all: Myloader

Myloader:
	gcc -o Myloader Myloader.c -lm

clean:
	rm Myloader
