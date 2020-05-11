CC=g++
FLAGS= -Wall -Wextra -pthread -g

all: main

################################################################################
#						MAIN DRIVER
################################################################################

main: main.cpp dac.cpp dac.h fib.h merge.h problem.h qsort.h task.h worker.h worker.cpp
	$(CC) $(FLAGS) -o main main.cpp dac.cpp dac.h fib.h merge.h problem.h qsort.h task.h worker.h worker.cpp

################################################################################
#						CLEAN
################################################################################

clean: 
	rm -rf *.o main