#ifndef FIB_H
#define FIB_H

#include <functional>
#include <vector>

auto fibDivide = [](const int& p, std::vector<int>& subps){
	subps.push_back(p - 1);
	subps.push_back(p - 2);
};

auto fibCombine = [](const std::vector<int>& subps, int& res){
	res = subps[0] + subps[1];
};

auto fibBase = [](const int& p, int& res){
	std::vector<int> dp(p + 2);
	dp[0] = 0;
	dp[1] = 1;
	for (int i = 2; i <= p; ++i) {
		dp[i] = dp[i - 1] + dp[i - 2];
	}
	res = dp[p];
};

auto fibThreshold = [](const int& t){
	return t < 5;
};

#endif