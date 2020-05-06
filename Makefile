srcFiles = $(wildcard *.c)
objFiles = $(srcFiles:.c=.o)

CC = gcc

libs = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

%.o : %.c 
	$(CC) -c $< -o $@ -Wall -Wpedantic 

all : $(objFiles)
	$(CC) $^ $(libs)

%_debug.o : %.c
	$(CC) -c -g $< -o $@ -Wall -Wpedantic

debug : $(objFiles:.o=_debug.o)
	$(CC) -g $^ $(libs)
	
.PHONY: clean
clean : 
	rm -f $(objFiles) $(objFiles:.o=_debug.o) a.out