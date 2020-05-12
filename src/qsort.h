/**
 * This header contains the lambda functions that define the qsort problem
 * in the form that is required by the DAC skeleton code. Four functions are
 * required (divde, combine, base, threshold)
 */

#ifndef QSORT_H
#define QSORT_H

#include <vector>
#include <algorithm>

#include "problem.h"

// function that defines the divide function for qsort
auto qsortDivide = [](const problem_t& p, std::vector<problem_t>& subps){
	auto partition = p.right;
	auto i = p.left - 1;

	for (auto it = p.left; it != p.right - 1; ++it) {
		if (*it < *partition) {
			i++;
			std::swap(*i, *it);
		}
	}

	std::swap(*(i + 1), *p.right);
	partition = i + 1;

	problem_t a = {p.left, partition - 1};
	problem_t b = {partition + 1, p.right};
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
	return t.right - t.left < 50; //size
};

#endif