#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>

#include "dac.h"
#include "fib.h"
#include "merge.h"
#include "qsort.h"

using namespace std;

int main(){
	int n = 1000000;
	int c = 1; // use std::thread::hardware_concurrency() for max cores

	for (int i = 10000; i <= n; i += 10000) {
		// vector problem for merge sort and qsort
		vector<int> v(i);
		iota(begin(v), end(v), 0);
		shuffle(begin(v), end(v), mt19937(random_device{}()));
		problem_t p {begin(v), end(v)};

		// DAC<int, int> dac(fibDivide, fibCombine, fibBase, fibThreshold, i, c);
	
		// DAC<problem_t, result_t> dac(mergeDivide, mergeCombine, mergeBase, mergeThreshold, p, c);

		p.right -= 1;
		DAC<problem_t, result_t> dac( qsortDivide, qsortCombine, qsortBase, qsortThreshold, p, c);

		auto start = chrono::high_resolution_clock::now();

		dac.compute();

		auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
		
		// cout << "result: " << dac.getResult() << endl;
		cout << "size: " << i << " duration: " << duration << endl;
	}

	return 0;
}