# Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# Define libraries to be linked (for example -lm)
LDLIBS = 

# Define set of implementation source files and object files
SRC = dict2.c list.c data.c 
SRC2 = dict1.c list.c data.c 

# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)
OBJ2 = $(SRC2:.c=.o)

# Define the executable name(s)
EXE = dict2
EXE2 = dict2

# the first target:
dict2: dict2.o data.o list.o
	gcc -o dict2 dict2.o data.o list.o

dict2.o: dict2.c 
	gcc -c dict2.c 

data.o: data.c data.h 
	gcc -c data.c 

list.o: list.c list.h  
	gcc -c list.c 

#second target
dict1: dict1.o data.o list.o
	gcc -o dict1 dict1.o data.o list.o


clean:
	rm -f *.o dict1 
