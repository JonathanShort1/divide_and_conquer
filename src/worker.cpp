#include "worker.h"
#include "problem.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>

template <typename ProblemType, typename ResultType>
Worker<ProblemType, ResultType>::Worker(int id,
										const divide_f_t& d,
										const combine_f_t& c,
										const base_f_t& b,
										const threshold_f_t& t)
: d_isComplete(false)
, d_id(id)
, d_m(new std::mutex())
, divide(d)
, combine(c)
, base(b)
, threshold(t)
{}

template <typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::workQueuePush(
					  const std::shared_ptr<Task<ProblemType, ResultType>>& t) {
	std::lock_guard<std::mutex> lock(*d_m);
	d_q.push(t);
}

template <typename ProblemType, typename ResultType>
std::shared_ptr<Task<ProblemType, ResultType>> 
							   Worker<ProblemType, ResultType>::workQueuePop() {
	std::lock_guard<std::mutex> lock(*d_m);
	if (d_q.empty()) {
		return nullptr;
	}

	auto t = d_q.front();
	d_q.pop();
	return t;
}

template <typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::work()
{
	// create vector used for random task stealing
	std::vector<int> v(d_workers.size());
	std::iota(begin(v), end(v), 0); // generate numbers 0 to n - 1
	std::random_device rd;
	std::mt19937 g(rd());

	// loop while the worker is not complete
	while(!d_isComplete) {
		// If any other worker has complete then so have we
		auto anyComplete = std::any_of(
			begin(d_workers),
			end(d_workers),
			[](std::shared_ptr<Worker<ProblemType, ResultType>> w){ return w->d_isComplete; }
		);

		if (anyComplete) {
			d_isComplete = true;
			break;
		}

		// While tasks in own queue
		while (!d_q.empty()) {
			auto task = workQueuePop();

			if (task) { // if fails then queue empty (possible from steal)
				if (task->d_isRoot && task->d_isComplete) {
					// root task is complete therefore so are we
					d_isComplete = true;
					break;
				} else {
					solveTask(task);
				}
			}
		}

		// Attempt to steal from another queue
		std::shuffle(begin(v), end(v), g); // shuffle indexes randomly

		for (auto it = begin(v); it != end(v); ++it) {
			auto w = d_workers[*it]; // access random worker
			if (w->d_id != d_id) {// not self

				if (w->d_isComplete) { // a worker has complete so we should as well
					d_isComplete = true;
					break;
				}
				auto task = w->workQueuePop();

				if (task) {
					if (task->d_isRoot && task->d_isComplete) {
						d_isComplete = true;
					} else {
						// std::cout << "Successful steal from worker " << w->d_id << " to " << d_id << std::endl;
						solveTask(task);
					}
					break; // should now have some tasks on own queue or worker complete
				}
			}
		}
		// Completed all own tasks, tried to steal a task, now test for completion and repeat
	}
}

template <typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::solveTask(std::shared_ptr<Task<ProblemType, ResultType>> task)
{
	// Waiting on children
	if (task->d_isWaiting && !task->d_isComplete) {
		if (task->d_childCompleteCount == task->d_numChild) {
			task->d_isComplete = true;
		} else {
			// Children have not yet complete, add to back of same queue
			workQueuePush(task);
			return;
		}
	}

	// All children have finished, post result to parent
	if (task->d_isComplete) {
		combine(task->d_results, task->d_result);

		if (!task->d_isRoot) {
			task->d_parent->d_childCompleteCount++;
			task->d_parent->d_results[task->d_index] = task->d_result;
		} else {
			workQueuePush(task); // push root task back to queue to signify end of computation
		}
		return;
	}

	const ProblemType& p = task->d_problem;
	ResultType& r = task->d_result;

	// Below threshold, solve sequentially
	if (threshold(p)) {
		base(p, r);
		task->d_result = r;
		task->d_isComplete = true;
		if (!task->d_isRoot) {
			task->d_parent->d_childCompleteCount++;
			task->d_parent->d_results[task->d_index] = task->d_result;
		} else {
			workQueuePush(task); // push root task back to queue to signify end of computation
		}
		return;
	}

	// Divide problem
	std::vector<ProblemType> ps;
	divide(p, ps);

	task->d_numChild = ps.size();
	task->d_results = std::vector<ResultType>(ps.size());
	
	// Create child tasks and push them to work queue
	for (size_t i = 0; i < ps.size(); ++i) {
		workQueuePush(
			std::make_shared<Task<ProblemType, ResultType>>(ps[i], task, i, false)
		);
	}

	// Child tasks created, now add back to queue and wait for completion
	task->d_isWaiting = true;
	workQueuePush(task);
}

// Types to allow
template Worker<int, int>::Worker(int id, const divide_f_t& d, const combine_f_t& c, const base_f_t& b, const threshold_f_t& t);
template Worker<problem_t, result_t>::Worker(int id, const divide_f_t& d, const combine_f_t& c, const base_f_t& b, const threshold_f_t& t);
template void Worker<int, int>::work();
template void Worker<problem_t, result_t>::work();