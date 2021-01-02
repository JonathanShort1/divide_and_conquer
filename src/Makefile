CC=g++
FLAGS= -Wall -Wextra -pthread -g

all: main pfib qsort mergesort

################################################################################
#						MAIN DRIVER
################################################################################

main: main.cpp dac.h fib.h merge.h problem.h qsort.h task.h worker.h worker.cpp
	$(CC) $(FLAGS) -o main main.cpp dac.h fib.h merge.h problem.h qsort.h task.h worker.h worker.cpp

pfib: benchmarks/pfib.cpp
	$(CC) $(FLAGS) -o benchmarks/pfib benchmarks/pfib.cpp

qsort: benchmarks/qsort.cpp
	$(CC) $(FLAGS) -o benchmarks/qsort benchmarks/qsort.cpp

mergesort: benchmarks/mergesort.cpp
	$(CC) $(FLAGS) -o benchmarks/mergesort benchmarks/mergesort.cpp

################################################################################
#						CLEAN
################################################################################

clean: 
	rm -rf *.o main benchmarks/pfib benchmarks/qsort benchmarks/mergesort