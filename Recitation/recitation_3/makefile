#makefile
#this makefile compiles the code helloworld.c into
#a program called helloworld
#which is the same as the command line:
# gcc -o helloworld helloworld.c
#but it breaks it into 2 steps which is very helpful in
#large projects

#the C compiler
CC = gcc
CFLAGS = -Wall -O2 -g

#this rule is to link the object code into an executable
helloworld: helloworld.o
	$(CC) -o helloworld helloworld.o

#this rule is to compile the source code
helloworld.o: helloworld.c
	$(CC) $(CFLAGS) -c helloworld.c

#this rule is to remove all object code and the executable
clean:
	rm -f *.o
	rm -f helloworld




