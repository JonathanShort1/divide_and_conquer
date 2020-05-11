#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>

#include "dac.h"
#include "fib.h"
#include "merge.h"
#include "qsort.h"

using namespace std;

int main(){

	DAC<int, int> dac(fibDivide, fibCombine, fibBase, fibThreshold, 10);

	// vector<int> v(100);
	// iota(begin(v), end(v), 0);
	// shuffle(begin(v), end(v), mt19937(random_device{}()));
	// problem_t p {begin(v), end(v)};
	
	// DAC<problem_t, result_t> dac(
	// 	mergeDivide,
	// 	mergeCombine,
	// 	mergeBase,
	// 	mergeThreshold,
	// 	p
	// );

	// p.right -= 1;
	// DAC<problem_t, result_t> dac(
	// 	qsortDivide,
	// 	qsortCombine,
	// 	qsortBase,
	// 	qsortThreshold,
	// 	p
	// );

	dac.compute();
	cout << "result: " << dac.getResult() << endl;
}