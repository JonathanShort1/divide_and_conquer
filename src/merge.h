#ifndef MERGE_H
#define MERGE_H

#include <functional>
#include <vector>
#include <algorithm>

#include "problem.h"

auto mergeDivide = [](const problem_t& p, std::vector<problem_t>& subps){
	auto mid = p.left + (p.right - p.left) / 2;
	Problem a = {p.left, mid};
	Problem b = {mid, p.right};
	subps.push_back(a);
	subps.push_back(b);
};

auto mergeCombine = [](const std::vector<result_t>& subps, result_t& res){
	int size = subps[1].right - subps[0].left;
	std::vector<int> tmp(size);
	std::vector<int>::iterator left = subps[0].left;
	std::vector<int>::iterator mid = subps[0].right;
	std::vector<int>::iterator right = mid;
	
	for (int i = 0; i < size; ++i) {
		if (left < mid && (right >= subps[1].right || *left <= *right)) {
			tmp[i] = *left;
			left++;
		} else {
			tmp[i] = *right;
			right++;
		}
	}

	std::copy(begin(tmp), end(tmp), subps[0].left);
	res.left = subps[0].left;
	res.right = subps[1].right;
};

auto mergeBase = [](const problem_t& p, result_t& res){
	std::sort(p.left, p.right);
	res = p;
};

auto mergeThreshold = [](const problem_t& t){
	return t.right - t.left < 50; //size
};

#endif