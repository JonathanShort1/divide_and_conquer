/**
 * This class represents the divide and conquer skeleton public API.  A client
 * of this skeleton code is required to provide 4 lambda functions that
 * represent the four stages of a divide and conquer algorithm.
 * 
 * The divide function splits the problem into sub problems.
 * The combine function collects the result from sub problems
 * The theshold function determines whether the problem should be divided
 * further or just solved. 
 * The base function solve a problem sequentially and is to be used once the
 * threshold has been met. 
 */

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
		// TYPE ALIASES
		typedef std::function<void(const ProblemType& , std::vector<ProblemType>&)> divide_f_t;
		typedef std::function<void(const std::vector<ResultType>&, ResultType&)> combine_f_t;
		typedef std::function<void(const ProblemType&, ResultType&)> base_f_t;
		typedef std::function<bool(const ProblemType&)> threshold_f_t;

		// DATA MEMBERS
		divide_f_t divide; // function used to divide a problem into sub problems
		combine_f_t combine;// function collects the result from sub problems
		base_f_t base; // function solve a problem sequentially, used once the threshold has been met
		threshold_f_t threshold; // function determines whether the problem should be divided further or just solved
		
		const ProblemType d_problem; // problem to be solved
		ResultType d_result; // result holder
		int d_numCores; // number of cores available on architecture
		std::vector<std::shared_ptr<Worker<ProblemType, ResultType>>> d_workers; // set of workers to use

	public:
		// constructor
		DAC(const divide_f_t& d,
			const combine_f_t& c,
			const base_f_t& b,
			const threshold_f_t& t,
			const ProblemType& p,
			const int nc);

		// This function returns the result calculated by the compute function.
		// It must be called after compute(), calling it before results in
		// undefined behaviour.
		const ResultType& getResult() { return d_result; }

		// This function starts the computation to solve the problem.  It
		// initialise the worker pool and creates the root task.  The root task
		// is pushed to one work queue and all the workers are started.  Once
		// all the workers have complete the result it posted to 'result'.
		void compute();
};

#endif