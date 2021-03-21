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
    int n = 100000;
    int numCores = 1; // use std::thread::hardware_concurrency() for max cores

    for (int i = 10000; i <= n; i += 10000) {
        // vector problem for merge sort and qsort
        vector<int> v(i);
        iota(begin(v), end(v), 0);
        shuffle(begin(v), end(v), mt19937(random_device{}()));
        problem_t p {begin(v), end(v)};

        // DAC<Fib::problem_t, Fib::result_t> dac(fibDivide,
        //                                        fibCombine,
        //                                        fibBase,
        //                                        fibThreshold,
        //                                        i,
        //                                        numCores);

        // DAC<MergeSort::problem_t, MergeSort::result_t> dac(mergeDivide,
        //                                                    mergeCombine,
        //                                                    mergeBase,
        //                                                    mergeThreshold,
        //                                                    p,
        //                                                    numCores);

        p.right -= 1;
        DAC<QSort::problem_t, QSort::result_t> dac(qsortDivide,
                                                   qsortCombine,
                                                   qsortBase,
                                                   qsortThreshold,
                                                   p,
                                                   numCores);

        auto start = chrono::high_resolution_clock::now();

        dac.compute();

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // cout << "result: " << dac.getResult() << endl;
        cout << "size: " << i << " duration: " << duration << endl;
    }

    return 0;
}