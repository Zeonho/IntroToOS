# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -lpthread

# the build target executable:
TARGET = totalSize

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c 

clean:
	$(RM) $(TARGET)
	$(RM) LOGFILE
	$(RM) -rf $(TARGET).dSYM
