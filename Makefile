CFLAGS=-Wall -Wextra -ggdb


all: bin demo

demo: bin demo.c 
	gcc demo.c $(CFLAGS) -o bin\demo

bin:
	mkdir .\bin