/**
 * This header contains the lambda functions that define the fibonacci problem
 * in the form that is required by the DAC skeleton code. Four functions are
 * required (divde, combine, base, threshold)
 */

#ifndef FIB_H
#define FIB_H

#include <vector>

// function that defines the divide function for fibonacci
auto fibDivide = [](const int& p, std::vector<int>& subps){
	subps.emplace_back(p - 1);
	subps.emplace_back(p - 2);
};

// function that defines the combine function for fibonacci
auto fibCombine = [](const std::vector<int>& subps, int& res){
	res = subps[0] + subps[1];
};

// function that defines the base function for fibonacci
auto fibBase = [](const int& p, int& res){
	std::vector<int> dp(p + 2);
	dp[0] = 0;
	dp[1] = 1;
	for (int i = 2; i <= p; ++i) {
		dp[i] = dp[i - 1] + dp[i - 2];
	}
	res = dp[p];
};

// function that defines the threshold function for fibonacci
auto fibThreshold = [](const int& t){
	return t < 10;
};

#endif