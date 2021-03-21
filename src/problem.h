/**
 * This header defines the problem and result type for the merge sort and qsort
 * problems.  They both require a reference to the beginning of the sub problem
 * and the end of the sub problem.  This is represented by a vector iterator.
 * 
 * The '<<' operator is also overloaded to allow for easier printing.  This is
 * not used for any computation.
 */

#ifndef PROBLEM_H
#define PROBLEM_H

#include <iostream>
#include <vector>

struct Problem {
    std::vector<int>::iterator left;
    std::vector<int>::iterator right;
};

inline
std::ostream& operator <<(std::ostream &out, const struct Problem& p)
{
    for (auto it = p.left; it != p.right; ++it) {
        out << *it << " ";
    }
    return out;
}

typedef struct Problem problem_t;
typedef struct Problem result_t;

#endif