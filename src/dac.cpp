#include "dac.h"
#include "problem.h"

template <typename ProblemType, typename ResultType>
void DAC<ProblemType, ResultType>::compute()
{
	solve(problem, result);
}

template <typename ProblemType, typename ResultType>
void DAC<ProblemType, ResultType>::solve(const ProblemType& p, ResultType& r) {

	if (threshold(p)) {
		base(p, r);
		return;
	}

	std::vector<ProblemType> ps;
	divide(p, ps);

	std::vector<ResultType> rs(ps.size());
	std::vector<std::thread> ts(ps.size());
	for(size_t i = 0; i < ps.size(); ++i) {
		ts[i] = std::thread( [&, i] { solve(ps[i], rs[i]); } );
	}

	for(size_t i = 0; i < ps.size(); ++i) {
		ts[i].join();
	}

	combine(rs, r);
}

template void DAC<int, int>::compute();
template void DAC<problem_t, result_t>::compute();