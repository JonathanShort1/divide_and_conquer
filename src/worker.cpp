#include "worker.h"
#include "problem.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>

template <typename Problem, typename Result>
std::shared_ptr<Task<Problem, Result>> Worker<Problem, Result>::workQueuePop()
{
    std::lock_guard<std::mutex> lock(d_m);
    if (d_q.empty()) {
        return nullptr;
    }

    auto t = d_q.front();
    d_q.pop();
    return t;
}

template <typename Problem, typename Result>
void Worker<Problem, Result>::work()
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
            [](std::shared_ptr<Worker<Problem, Result>> w){
                return w->d_isComplete;
            }
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

        for (const int index : v) {
            auto w = d_workers[index]; // access random worker
            if (w->d_id == d_id) {// not self
                continue;
            }

            if (w->d_isComplete) {
                // a worker has complete so we should as well
                d_isComplete = true;
                break;
            }

            auto task = w->workQueuePop();
            if (task) {
                if (task->d_isRoot && task->d_isComplete) {
                    d_isComplete = true;
                } else {
                    solveTask(task); // called here before it's stolen
                }
                // should now have some tasks on own queue or worker
                // complete
                break;
            }
        }
        // Completed all own tasks, tried to steal a task, now test for
        // completion and repeat
    }
}

template <typename Problem, typename Result>
void Worker<Problem, Result>::solveTask(
                                    std::shared_ptr<Task<Problem, Result>> task)
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
            // push root task back to queue to signify end of computation
            workQueuePush(task);
        }
        return;
    }

    // Below threshold, solve sequentially
    if (threshold(task->d_problem)) {
        base(task->d_problem, task->d_result);
        task->d_isComplete = true;
        if (!task->d_isRoot) {
            task->d_parent->d_childCompleteCount++;
            task->d_parent->d_results[task->d_index] = task->d_result;
        } else {
            // push root task back to queue to signify end of computation
            workQueuePush(task);
        }
        return;
    }

    // Divide problem
    std::vector<Problem> ps;
    divide(task->d_problem, ps);

    task->d_numChild = ps.size();
    task->d_results = std::vector<Result>(ps.size());

    // Create child tasks and push them to work queue
    for (size_t i = 0; i < ps.size(); ++i) {
        workQueuePush(
                std::make_shared<Task<Problem, Result>>(ps[i], task, i, false));
    }

    // Child tasks created, now add back to queue and wait for completion
    task->d_isWaiting = true;
    workQueuePush(task);
}

// Types to allow
template void Worker<int, int>::work();
template void Worker<problem_t, result_t>::work();