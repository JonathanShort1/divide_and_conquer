/**
 * This header contains the lambda functions that define the qsort problem
 * in the form that is required by the DAC skeleton code. Four functions are
 * required (divde, combine, base, threshold)
 */

#ifndef QSORT_H
#define QSORT_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "problem.h"

// function that defines the divide function for qsort
auto qsortDivide = [](const problem_t& p, std::vector<problem_t>& subps){
	auto partition = p.right;
	auto i = p.left - 1;

	for (auto it = p.left; it != p.right; ++it) {
		if (*it < *partition) {
			i++;
			std::swap(*i, *it);
		}
	}

	std::swap(*(i + 1), *partition);
	partition = i + 1;

	problem_t a = {p.left, partition - 1};
	problem_t b = {partition + 1, p.right};

	if (std::distance(a.left, a.right) < 0) a.left = a.right;
	if (std::distance(b.left, b.right) < 0) b.left = b.right;
	subps.push_back(a);
	subps.push_back(b);
};

// function that defines the combine function for qsort
auto qsortCombine = [](const std::vector<result_t>& subps, result_t& res){
	res.left = subps[0].left;
	res.right = subps[1].right;
};

// function that defines the base function for qsort
auto qsortBase = [](const problem_t& p, result_t& res){
	std::sort(p.left, p.right);
	res = p;
};

// function that defines the threshold function for qsort
auto qsortThreshold = [](const problem_t& t){
	return std::distance(t.left, t.right) < 2000; //size
};

#endif