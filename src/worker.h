#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <memory>
#include <mutex>
#include <functional>
#include <queue>

#include "task.h"

template <typename ProblemType, typename ResultType>
class Worker {
	private:
		std::queue<std::shared_ptr<Task<ProblemType, ResultType>>> q;
		std::mutex m;

		bool isComplete;

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

		void solveTask(std::shared_ptr<Task<ProblemType, ResultType>> t);
	
	public:
		Worker(
			const divide_f_t& d,
			const combine_f_t& c,
			const base_f_t& b,
			const threshold_f_t& t
			)
			: divide(d)
			, combine(c)
			, base(b)
			, threshold(t)
			{}

		void work();

		void workQueuePush(std::shared_ptr<Task<ProblemType, ResultType>> t);
		std::shared_ptr<Task<ProblemType, ResultType>> workQueuePop();
};

#endif