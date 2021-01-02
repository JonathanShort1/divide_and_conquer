/**
 * This class represents a task.  It is a unit of work to be performed by a
 * worker.  It has a 'problem' to be solved and the result is stored in 'result'.
 * 
 * If a 'problem' can be subdivided then the number of children created is
 * recored and a results vector is created.  A task is set to waiting while the
 * children asre completing the task.  Once all the children have complete a
 * result is put into 'result'.
 * 
 * If the task is the child of another task then it has a link to its 'parent'
 * along with the 'index' where it is to post its 'result' in its 'parent's'
 * result vector.
 * 
 * The isRoot field is important. The signal for all the workers to terminate is
 * when the root task has complete.
 */

#ifndef TASK_H
#define TASK_H

#include <memory>
#include <vector>

template <typename Problem, typename Result>
class Task {
  public:
    bool                        d_isComplete; // Has result field been filled
    bool                        d_isWaiting; // is waiting on its children
    bool                        d_isRoot; // Is this the root task (Important!)
    const int                   d_index; // index in parent's results vector
    const std::shared_ptr<Task> d_parent; // link to parent
    size_t                      d_numChild; // number of child tasks
    size_t                      d_childCompleteCount; // num children completed
    const Problem               d_problem; // problem to solve
    Result                      d_result; // result to return to parent of DAC
    std::vector<Result>         d_results; // results vector for children tasks

    Task(const Problem p, const std::shared_ptr<Task> par, const int i,
                                                                    bool isRoot)
    : d_isComplete(false)
    , d_isWaiting(false)
    , d_isRoot(isRoot)
    , d_index(i)
    , d_parent(par)
    , d_numChild(0)
    , d_childCompleteCount(0)
    , d_problem(p)
    {}

    // This function returns the 'result' calculated by this task for the
    // 'problem'
    Result& getResult() { return d_result; }
};

#endif