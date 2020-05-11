#include <memory>
#include <iostream>

#include "dac.h"
#include "problem.h"

template <typename ProblemType, typename ResultType>
void DAC<ProblemType, ResultType>::compute()
{
	// set up new threads per worker
	// crate worker class

	/**
	 * worker pulls from queue
	 * if task children have not complete put to back
	 * if task children have all comlete combine and remove
	 * save result in parent
	 */

	std::shared_ptr<Task<ProblemType, ResultType>> p(nullptr); // dummy parent
	auto t = std::make_shared<Task<ProblemType, ResultType>>(problem, p, 0, true);
	worker.workQueuePush(t);

	std::thread w1 = std::thread([&] {worker.work(); } );
	w1.join();

	std::cout << t->problem << std::endl;


	//signal threads to exit

	result = t->getResult();
}

template void DAC<int, int>::compute();
template void DAC<problem_t, result_t>::compute();