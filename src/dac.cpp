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
								  const ProblemType& p,
								  const int nc)
: divide(d)
, combine(c)
, base(b)
, threshold(t)
, d_problem(p)
, d_numCores(nc)
{}

template <typename ProblemType, typename ResultType>
void DAC<ProblemType, ResultType>::compute()
{
	// Create workers
	for (int i = 0; i < d_numCores; ++i) {
		d_workers.push_back(std::make_shared<Worker<ProblemType, ResultType>>(i, divide, combine, base, threshold));
	}

	// set vector of workers for each worker
	for (int i = 0; i < d_numCores; ++i) {
		d_workers[i]->setWorkerVector(d_workers);
	}

	// Create root task
	std::shared_ptr<Task<ProblemType, ResultType>> p(nullptr); // dummy parent
	auto task = std::make_shared<Task<ProblemType, ResultType>>(d_problem, p, 0, true);
	
	// Push root task to first worker
	d_workers[0]->workQueuePush(task);

	// Set all worker running
	std::vector<std::thread> threads;
	for (int i = 0; i < d_numCores; ++i) {
		threads.push_back(std::thread([this, i] { d_workers[i]->work(); }));
	}

	// wait for all workers to complete
	for (int i = 0; i < d_numCores; ++i) {
		threads[i].join();
	}

	// get result from root task
	d_result = task->getResult();
}

// Types to allow
template DAC<int, int>::DAC(const divide_f_t& d, const combine_f_t& c, const base_f_t& b, const threshold_f_t& t, const int& p, const int nc);
template DAC<problem_t, result_t>::DAC(const divide_f_t& d, const combine_f_t& c, const base_f_t& b, const threshold_f_t& t, const problem_t& p, const int nc);
template void DAC<int, int>::compute();
template void DAC<problem_t, result_t>::compute();