/**
 * This header contains the lambda functions that define a merge sort problem
 * in the form that is required by the DAC skeleton code. Four functions are
 * required (divde, combine, base, threshold)
 */

#ifndef MERGE_H
#define MERGE_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "problem.h"

// function that defines the divide function for merge sort
auto mergeDivide = [](const problem_t& p, std::vector<problem_t>& subps){
auto mid = p.left + ((p.right - p.left) / 2);
problem_t a = { p.left, mid };
problem_t b = { mid, p.right };
    subps.emplace_back(a);
    subps.emplace_back(b);
};

// function that defines the combine function for merge sort
auto mergeCombine = [](const std::vector<result_t>& subps, result_t& res){
// size_t size = subps[1].right - subps[0].left;
    auto lsize = std::distance(subps[0].left, subps[0].right);
    auto rsize = std::distance(subps[1].left, subps[1].right);
    std::vector<int> l(lsize);
    std::vector<int> r(rsize);

    std::copy(begin(l), end(l), subps[0].left);
    std::copy(begin(r), end(r), subps[1].left);

    std::vector<int>::iterator k = subps[0].left;
    int i = 0;
    int j = 0;

    while(i < lsize && j < rsize) {
        if (l[i] <= r[i]) {
            *k = l[i];
            i++;
        } else {
            *k = r[j];
            j++;
    }
}

while (i < lsize) {
    *k = l[i];
    i++;
    k++;
}

while (j < rsize) {
    *k = r[i];
    j++;
    k++;

}

res.left = subps[0].left;
res.right = subps[1].right;
};

// function that defines the base function for merge sort
auto mergeBase = [](const problem_t& p, result_t& res){
    std::sort(p.left, p.right);
    res = p;
};

// function that defines the threshold function for merge sort
    auto mergeThreshold = [](const problem_t& t){
    return std::distance(t.left, t.right) < 2000; //size
};

#endif