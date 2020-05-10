#ifndef QSORT_H
#define QSORT_H

#include <functional>
#include <vector>
#include <algorithm>

#include "problem.h"

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

	Problem a = {p.left, partition - 1};
	Problem b = {partition + 1, p.right};
	subps.push_back(a);
	subps.push_back(b);
};

auto qsortCombine = [](const std::vector<result_t>& subps, result_t& res){
	res.left = subps[0].left;
	res.right = subps[1].right;
};

auto qsortBase = [](const problem_t& p, result_t& res){
	std::sort(p.left, p.right);
	res = p;
};

auto qsortThreshold = [](const problem_t& t){
	return t.right - t.left < 50; //size
};

#endif