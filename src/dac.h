#ifndef DAC_H
#define DAC_H

#include <vector>
#include <functional>
#include <thread>

template <typename ProblemType, typename ResultType>
class DAC {
	private:

		// TYPE ALIASES
		typedef std::function<void(const ProblemType& , std::vector<ProblemType>&)> divide_f_t;
		typedef std::function<void(const std::vector<ResultType>&, ResultType&)> combine_f_t;
		typedef std::function<void(const ProblemType&, ResultType&)> base_f_t;
		typedef std::function<bool(const ProblemType&)> threshold_f_t;

		// DATA MEMBERS
		divide_f_t divide;
		combine_f_t combine;
		base_f_t base;
		threshold_f_t threshold;

		const ProblemType& problem;
		ResultType result;

		int numCores;

		void solve(const ProblemType& p, ResultType& r);

	public:
		DAC(const divide_f_t& d,
			const combine_f_t& c,
			const base_f_t& b,
			const threshold_f_t& t,
			const ProblemType& p)
			: divide(d),
			combine(c),
			base(b),
			threshold(t),
			problem(p),
			numCores(std::thread::hardware_concurrency())
			{}

		const ResultType& getResult() { return result; }

		void compute();
};

#endif