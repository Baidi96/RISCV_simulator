
all: Myloader

Myloader:
	gcc -o Myloader Myloader.c

clean:
	rm Myloader
