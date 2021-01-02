/**
 * This class represents a worker.  A worker has a work queue of tasks.  A
 * worker pulls tasks from its queue and solves them. If a worker's work queue
 * is empty but the root task is not complete then a worker may steal work from
 * another work queue from the pool of workers.
 */

#ifndef WORKER_H
#define WORKER_H

#include <functional>
#include <memory>
#include <mutex>
#include <queue>

#include "task.h"

template <typename Problem, typename Result>
class Worker {
  private:
    bool d_isComplete; // Set when this task is complete
    int d_id; // ID of worker (used in work stealing)
    std::vector<std::shared_ptr<Worker<Problem, Result>>> d_workers; // worker pool
    std::queue<std::shared_ptr<Task<Problem, Result>>> d_q; // work queue
    std::mutex d_m; // mutex to ensure thread safety for queue operation

    // TYPE ALIASES
    typedef std::function<void(const Problem&, std::vector<Problem>&)> divide_f_t;
    typedef std::function<void(const std::vector<Result>&, Result&)> combine_f_t;
    typedef std::function<void(const Problem&, Result&)> base_f_t;
    typedef std::function<bool(const Problem&)> threshold_f_t;

    // Functions passed to the DAC skeleton
    divide_f_t    divide;
    combine_f_t   combine;
    base_f_t      base;
    threshold_f_t threshold;

    // This function solves a task from the work queue.
    // If a task is waiting but not yet complete then it checks if all its
    // children have complete.  If so then set task to complete, otherwise
    // push task back to work queue.
    // If a task has complete then combine all the results from children and
    // post result to parent task.  If the task is the root task then push
    // back onto work queue. (The next worker to pull this task will set
    // itself to complete, causing all workers to termintate).
    // If the task is not waiting or complete then it is a "fresh" task.
    // Check if the problem is below the threshold.  If it is then solve it,
    // set the task to complete and post result to parent.  If it is not
    // divide the work and create child tasks.  Then set task to waiting and
    // push back onto work queue.
    void solveTask(std::shared_ptr<Task<Problem, Result>> t);

    // This function pops a task from the work queue.  If the queue is empty
    // then this operation will return a nullptr.  The return value must be
    // checked.
    std::shared_ptr<Task<Problem, Result>> workQueuePop();

public:
    // Constructor
    Worker(int id, const divide_f_t& d, const combine_f_t& c, const base_f_t& b,
                                                         const threshold_f_t& t)
    : d_isComplete(false)
    , d_id(id)
    , divide(d)
    , combine(c)
    , base(b)
    , threshold(t)
    {}

    // This is the function that controls the work flow of a worker
    // First a vector of integers is initialised and a random device.  This
    // is used to access the worker pool.
    // The worker enters a loop until it is complete.
    // If any other worker in the work pool is complete then the root task
    // has complete so this worker can also complete.
    // While a workers own work queue is not empty, pull tasks and solve.
    // If a workers own work queue is empty then attempt to steal work.  It
    // does this by shuffling the vector of ints to ensure random stealing.
    // It then iterates the worker pool and attempt to steal work.  Once one
    // task is stolen it goes back to start of work flow again.
    void work();

    // This function sets the worker pool.  This is done after construction
    // as not all the workers will have been created yet.  This must be
    // called before 'work()'.
    void setWorkerVector(
                       std::vector<std::shared_ptr<Worker<Problem, Result>>> ws)
    {
        d_workers = ws;
    }

    // This function pushes a task to the work queue.  A root task must be
    // pushed to one worker in the work pool before 'work()' is called.
    void workQueuePush(const std::shared_ptr<Task<Problem, Result>>& t)
    {
        std::lock_guard<std::mutex> lock(d_m);
        d_q.push(t);
    }
};

#endif