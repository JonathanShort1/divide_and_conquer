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

	// DAC<int, int> dac(fibDivide,
	// 				fibCombine,
	// 				fibBase,
	// 				fibThreshold,
	// 				40);

	// vector problem for merge sort and qsort
	unsigned int n = 2000000;
	vector<int> v(n);
	iota(begin(v), end(v), 0);
	shuffle(begin(v), end(v), mt19937(random_device{}()));
	problem_t p {begin(v), end(v)};
	
	DAC<problem_t, result_t> dac(
		mergeDivide,
		mergeCombine,
		mergeBase,
		mergeThreshold,
		p
	);

	// p.right -= 1;
	// DAC<problem_t, result_t> dac(
	// 	qsortDivide,
	// 	qsortCombine,
	// 	qsortBase,
	// 	qsortThreshold,
	// 	p
	// );


	auto start = chrono::high_resolution_clock::now();

	dac.compute();

	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

	// cout << "result: " << dac.getResult() << endl;
	cout << "duration: " << duration << endl;

	return 0;
}