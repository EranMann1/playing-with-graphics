CFLAGS = -lm

ifeq ($(debug), 1)
	CFLAGS = -g -lm
endif

example:simple_circle.c
	gcc -o example simple_circle.c 

cube: graphics.h graphics.c makefile
	gcc -o cube graphics.c $(CFLAGS)