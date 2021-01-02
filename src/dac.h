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

#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "problem.h"
#include "task.h"
#include "worker.h"

template <typename Problem, typename Result>
class DAC {
  private:
    // TYPE ALIASES
    typedef std::function<void(const Problem&, std::vector<Problem>&)> divide_f_t;
    typedef std::function<void(const std::vector<Result>&, Result&)> combine_f_t;
    typedef std::function<void(const Problem&, Result&)> base_f_t;
    typedef std::function<bool(const Problem&)> threshold_f_t;

    // DATA MEMBERS
    divide_f_t    divide;
    combine_f_t   combine;
    base_f_t      base;
    threshold_f_t threshold;

    const Problem   d_problem;  // problem to be solved
    Result          d_result;   // result holder
    const int       d_numCores; // number of cores available on architecture
    std::vector<std::shared_ptr<Worker<Problem, Result>>> d_workers;

  public:
    // constructor
    DAC(const divide_f_t& d, const combine_f_t& c, const base_f_t& b,
                         const threshold_f_t& t, const Problem& p, const int nc)
    : divide(d)
    , combine(c)
    , base(b)
    , threshold(t)
    , d_problem(p)
    , d_numCores(nc)
    {}

    // This function returns the result calculated by the compute function.
    // It must be called after compute(), calling it before results in
    // undefined behaviour.
    const Result& getResult() { return d_result; }

    // This function starts the computation to solve the problem.  It
    // initialise the worker pool and creates the root task.  The root task
    // is pushed to one work queue and all the workers are started.  Once
    // all the workers have complete the result it posted to 'result'.
    void compute()
    {
        // Create workers
        for (int i = 0; i < d_numCores; ++i) {
            d_workers.emplace_back(
                std::make_shared<Worker<Problem, Result>>(i,
                                                          divide,
                                                          combine,
                                                          base,
                                                          threshold));
        }

        // set vector of workers for each worker
        for (int i = 0; i < d_numCores; ++i) {
            d_workers[i]->setWorkerVector(d_workers);
        }

        // Create root task
        std::shared_ptr<Task<Problem, Result>> dummyParent(nullptr);
        auto task = std::make_shared<Task<Problem, Result>>(d_problem,
                                                            dummyParent,
                                                            0,
                                                            true);

        // Push root task to first worker
        d_workers[0]->workQueuePush(task);

        // Set all worker running
        std::vector<std::thread> threads;
        for (int i = 0; i < d_numCores; ++i) {
            threads.emplace_back(std::thread([this, i] {
                d_workers[i]->work();
            }));
        }

        // wait for all workers to complete
        for (int i = 0; i < d_numCores; ++i) {
            threads[i].join();
        }

        // get result from root task
        d_result = task->getResult();
    }
};

#endif