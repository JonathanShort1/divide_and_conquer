#ifndef TASK_H
#define TASK_H

#include <memory>
#include <vector>

template <typename ProblemType, typename ResultType>
class Task {
	private:

	public:
		bool isComplete;
		bool isWaiting;
		bool isRoot;
		const ProblemType problem;
		const std::shared_ptr<Task> parent;
		const int index;
		size_t childCompleteCount;
		ResultType result;
		size_t numChild;
		std::vector<ResultType> results;

		Task(
			const ProblemType p,
			const std::shared_ptr<Task> par,
			const int i,
			bool isRoot
		)
		: isComplete(false)
		, isWaiting(false)
		, isRoot(isRoot)
		, problem(p)
		, parent(par)
		, index(i)
		, childCompleteCount(0)
		, numChild(0)
		{}

		ResultType& getResult() { return result; }
};

#endif