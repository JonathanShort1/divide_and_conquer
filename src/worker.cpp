#include "worker.h"
#include "problem.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>

template <typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::workQueuePush(
						const std::shared_ptr<Task<ProblemType, ResultType>>& t) {
	std::lock_guard<std::mutex> lock(*m);
	q.push(t);
}

template <typename ProblemType, typename ResultType>
std::shared_ptr<Task<ProblemType, ResultType>> 
								Worker<ProblemType, ResultType>::workQueuePop() {
	std::lock_guard<std::mutex> lock(*m);
	if (q.empty()) {
		return nullptr;
	}

	auto t = q.front();
	q.pop();
	return t;
}

template <typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::work()
{
	std::vector<int> v(workers.size());
	std::iota(begin(v), end(v), 0); // generate numbers 0 to n - 1
	std::random_device rd;
	std::mt19937 g(rd());

	while(!isComplete) {
		// If any other worker has complete then so have we
		auto anyComplete = std::any_of(
					begin(workers),
					end(workers),
					[](std::shared_ptr<Worker<ProblemType, ResultType>> w){ return w->isComplete; }
				);

		if (anyComplete) {
			isComplete = true;
			break;
		}

		// While tasks in own queue
		while (!q.empty()) {
			auto t = workQueuePop();

			if (t) { // if fails then task was stolen
				if (t->isRoot && t->isComplete) {
					isComplete = true;
				} else {
					solveTask(t);
				}
			}
		}

		// Attempt to steal from another queue
		std::shuffle(begin(v), end(v), g); // shuffle indexes randomly

		for (auto it = begin(v); it != end(v); ++it) {
			auto w = workers[*it]; // access random worker
			if (w->id != id) {// not self

				if (w->isComplete) { // a worker has complete so we should as well
					isComplete = true;
					break;
				}
				auto t = w->workQueuePop();

				if (t) {
					if (t->isRoot && t->isComplete) {
						isComplete = true;
					} else {
						std::cout << "Successful steal" << std::endl;
						solveTask(t);
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
	if (task->isWaiting && !task->isComplete) {
		if (task->childCompleteCount == task->numChild) {
			task->isComplete = true;
		} else {
			// Children have not yet complete, add to back of same queue
			workQueuePush(task);
			return;
		}
	}

	// All children have finished, post result to parent
	if (task->isComplete) {
		combine(task->results, task->result);

		if (!task->isRoot) {
			task->parent->childCompleteCount++;
			task->parent->results[task->index] = task->result;
		} else {
			workQueuePush(task); // push root task back to queue to signify end of computation
		}
		return;
	}

	const ProblemType& p = task->problem;
	ResultType& r = task->result;

	// Below threshold, solve sequentially
	if (threshold(p)) {
		base(p, r);
		task->result = r;
		task->isComplete = true;
		if (!task->isRoot) {
			task->parent->childCompleteCount++;
			task->parent->results[task->index] = task->result;
		} else {
			workQueuePush(task); // push root task back to queue to signify end of computation
		}
		return;
	}

	// Divide problem
	std::vector<ProblemType> ps;
	divide(p, ps);

	task->numChild = ps.size();

	task->results = std::vector<ResultType>(ps.size());
	
	for (size_t i = 0; i < ps.size(); ++i) {
		workQueuePush(
			std::make_shared<Task<ProblemType, ResultType>>(ps[i], task, i, false)
		);
	}

	// Child tasks created, now add back to queue and wait for completion
	task->isWaiting = true;
	workQueuePush(task);
}

template void Worker<int, int>::work();
template void Worker<problem_t, result_t>::work();