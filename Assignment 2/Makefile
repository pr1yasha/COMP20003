# Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# Define libraries to be linked (for example -lm)
LDLIBS = 

# Define set of implementation source files and object files
SRC = dict3.c list.c data.c quadTree.c
SRC2 = dict4.c list.c data.c quadTree.c

# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)
OBJ2 = $(SRC2:.c=.o)

# Define the executable name(s)
EXE = dict3
EXE2 = dict2

# the first target:
dict3: dict3.o data.o list.o quadTree.o
	gcc -o dict3 dict3.o data.o list.o quadTree.o

dict2.o: dict2.c
	gcc -c dict2.c 

data.o: data.c data.h 
	gcc -c data.c 

list.o: list.c list.h  
	gcc -c list.c 

quadTree.o: quadTree.c quadTree.h  
	gcc -c quadTree.c 

dict4: dict4.o data.o list.o quadTree.o
	gcc -o dict4 dict4.o data.o list.o quadTree.o

clean:
	rm -f *.o dict4 
