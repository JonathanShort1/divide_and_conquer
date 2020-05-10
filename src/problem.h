#ifndef PROBLEM_H
#define PROBLEM_H

#include <iostream>
#include <vector>

struct Problem {
	std::vector<int>::iterator left;
	std::vector<int>::iterator right;
};

inline std::ostream& operator <<(std::ostream &out, const struct Problem& p) {
		for (auto it = p.left; it != p.right; ++it) {
			out << *it << " ";
		}
		return out;
	}

typedef struct Problem problem_t;
typedef struct Problem result_t;

#endif