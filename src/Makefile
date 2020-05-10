CC=g++
FLAGS= -Wall -Wextra -pthread -g

all: main

################################################################################
#						MAIN DRIVER
################################################################################

main: main.cpp dac.cpp dac.h fib.h merge.h problem.h qsort.h
	$(CC) $(FLAGS) -o main main.cpp dac.cpp dac.h fib.h merge.h problem.h qsort.h

################################################################################
#						CLEAN
################################################################################

clean: 
	rm -rf *.o main