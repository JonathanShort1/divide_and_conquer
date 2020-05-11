CPPC=g++
CC = gcc
FLAGS= -Wall -Wextra -pthread -g

all: main pfib qsort mergesort

################################################################################
#						MAIN DRIVER
################################################################################

main: main.cpp dac.cpp dac.h fib.h merge.h problem.h qsort.h task.h worker.h worker.cpp
	$(CPPC) $(FLAGS) -o main main.cpp dac.cpp dac.h fib.h merge.h problem.h qsort.h task.h worker.h worker.cpp

pfib: benchmarks/pfib.c
	$(CC) $(FLAGS) -o benchmarks/pfib benchmarks/pfib.c

qsort: benchmarks/qsort.c
	$(CC) $(FLAGS) -o benchmarks/qsort benchmarks/qsort.c

mergesort: benchmarks/mergesort.c
	$(CC) $(FLAGS) -o benchmarks/mergesort benchmarks/mergesort.c

################################################################################
#						CLEAN
################################################################################

clean: 
	rm -rf *.o main pfib qsort mergesort