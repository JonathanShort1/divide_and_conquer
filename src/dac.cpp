#include "dac.h"
#include "problem.h"

#include <iostream>
#include <memory>
#include <thread>

template <typename ProblemType, typename ResultType>
DAC<ProblemType, ResultType>::DAC(const divide_f_t& d,
								  const combine_f_t& c,
								  const base_f_t& b,
								  const threshold_f_t& t,
								  const ProblemType& p)
: divide(d)
, combine(c)
, base(b)
, threshold(t)
, problem(p)
, numCores(std::thread::hardware_concurrency())
{}

template <typename ProblemType, typename ResultType>
void DAC<ProblemType, ResultType>::compute()
{
	/**
	 * worker pulls from queue
	 * if task children have not complete put to back
	 * if task children have all comlete combine and remove
	 * save result in parent
	 */

	// Create workers
	for (int i = 0; i < numCores; ++i) {
		workers.push_back(
			std::make_shared<Worker<ProblemType, ResultType>>(i, divide, combine, base, threshold)
		);
	}

	// build vector of workers for each worker
	for (int i = 0; i < numCores; ++i) {
		workers[i]->setWorkerVector(workers);
	}

	// Create root task
	std::shared_ptr<Task<ProblemType, ResultType>> p(nullptr); // dummy parent
	auto t = std::make_shared<Task<ProblemType, ResultType>>(problem, p, 0, true);
	
	// Push root task to first worker
	workers[0]->workQueuePush(t);

	// Set all worker running
	std::vector<std::thread> threads;
	for (int i = 0; i < numCores; ++i) {
		threads.push_back(std::thread([this, i] { workers[i]->work(); }));
	}

	// wait for all workers to complete
	for (int i = 0; i < numCores; ++i) {
		threads[i].join();
	}

	// get result from root task
	result = t->getResult();
}

template <typename ProblemType, typename ResultType>
void DAC<ProblemType, ResultType>::pushToVector(const int index)
{
	
}

// Types to allow
template DAC<int, int>::DAC(const divide_f_t& d, const combine_f_t& c, const base_f_t& b, const threshold_f_t& t, const int& p);
template DAC<problem_t, result_t>::DAC(const divide_f_t& d, const combine_f_t& c, const base_f_t& b, const threshold_f_t& t, const problem_t& p);
template void DAC<int, int>::compute();
template void DAC<problem_t, result_t>::compute();