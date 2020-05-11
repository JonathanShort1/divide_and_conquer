#include "worker.h"
#include "problem.h"

#include <iostream>

template <typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::workQueuePush(std::shared_ptr<Task<ProblemType, ResultType>> t) {
	std::lock_guard<std::mutex> lock(m);

	std::cout << "push " << t->problem << std::endl;
	q.push(t);
}

template <typename ProblemType, typename ResultType>
std::shared_ptr<Task<ProblemType, ResultType>> Worker<ProblemType, ResultType>::workQueuePop() {
	std::lock_guard<std::mutex> lock(m);
	if (q.empty()) {
		return nullptr;
	}

	auto t = q.front();
	std::cout << "pop " << t->problem << t.use_count() << std::endl;

	q.pop();
	return t;
}

template <typename ProblemType, typename ResultType>
void Worker<ProblemType, ResultType>::work()
{
	while (!q.empty()) {
		std::shared_ptr<Task<ProblemType, ResultType>> t = workQueuePop();

		if (!t) {
			std::cout << "stolen" << std::endl;
		}

		std::cout << "solve task " << t->problem << " " << t->isWaiting << " " << t->isComplete << std::endl;

		solveTask(t);
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
		}
		return;
	}

	// Divide problem
	std::vector<ProblemType> ps;
	divide(p, ps);

	task->numChild = ps.size();

	task->results = std::vector<ResultType>(ps.size());
	
	for (size_t i = 0; i < ps.size(); ++i) {
		// std::shared_ptr<Task<ProblemType, ResultType>> t =
			// std::shared_ptr<Task<ProblemType, ResultType>>(new Task<ProblemType, ResultType>(ps[i], task, i, false));
		workQueuePush(
			std::make_shared<Task<ProblemType, ResultType>>(ps[i], task, i, false)
		);
	}

	auto t = workQueuePop();
	std::cout << "solve task " << t->problem << " " << t->isWaiting << " " << t->isComplete << std::endl;
	workQueuePush(t);

	// Child tasks created, now add back to queue and wait for completion
	task->isWaiting = true;
	// workQueuePush(task);
}

template void Worker<int, int>::work();
template void Worker<problem_t, result_t>::work();