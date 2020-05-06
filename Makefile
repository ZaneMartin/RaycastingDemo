srcFiles = $(wildcard *.c)
objFiles = $(srcFiles:.c=.o)

CC = gcc

libs = -lSDL2 -lSDL2_image

%.o : %.c 
	$(CC) -c $< -o $@ -Wall -Wpedantic 

all : $(objFiles)
	$(CC) $^ $(libs) 