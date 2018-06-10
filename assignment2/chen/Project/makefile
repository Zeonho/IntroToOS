#makefile
#this makefile compiles the code chen4566Project.c into
#a program called chen4566Project
#which is the same as the command line:
# gcc -o chen4566Project chen4566Project.c
#but it breaks it into 2 steps which is very helpful in
#large projects

#the C compiler
CC = gcc
CFLAGS = -std=c99 -Wall -I.

#this rule is to link the object code into an executable
chen4566Project: chen4566Project.o
	$(CC) -o chen4566Project chen4566Project.o

#this rule is to compile the source code
chen4566Project.o: chen4566Project.c
	$(CC) $(CFLAGS) -c chen4566Project.c

#this rule is to remove all object code and the executable
clean:
	rm -f *.o
	rm -f chen4566Project
