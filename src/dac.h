#ifndef DAC_H
#define DAC_H

#include <memory>
#include <vector>
#include <functional>

#include "task.h"
#include "worker.h"

template <typename ProblemType, typename ResultType>
class DAC {
	private:

	public:

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

		const ProblemType problem;
		ResultType result;

		int numCores;
		std::vector<std::shared_ptr<Worker<ProblemType, ResultType>>> workers;

		DAC(const divide_f_t& d,
			const combine_f_t& c,
			const base_f_t& b,
			const threshold_f_t& t,
			const ProblemType& p);

		const ResultType& getResult() { return result; }

		void compute();

		void pushToVector(const int i);
};

#endif