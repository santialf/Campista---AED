#make file example

CC = gcc #compiler

TARGET = campista #target file name

all: 
	$(CC) -Wall -std=c99 -O3 -o $(TARGET) main.c structInfo.c structInfo.h fileOp.c fileOp.h chainEffect.c chainEffect.h stack.c stack.h recursiveAndABC.c recursiveAndABC.h

clean: 
	rm $(TARGET)
