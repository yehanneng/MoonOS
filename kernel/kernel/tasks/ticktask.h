//
// Created by Tangruiwen on 2017/3/4.
//

#ifndef MOONOS_TICKTASK_H
#define MOONOS_TICKTASK_H

#include <kernel/tasks/task.h>

#ifdef __cplusplus

class ClockInterruptHandler;
class TickTask: public AbstractTask {
public:
    TickTask();
    virtual ~TickTask();
    virtual void run() override ;

private:
    ClockInterruptHandler* _clockHandler;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

void tick_task_main();

#ifdef __cplusplus
};
#endif

#endif //MOONOS_TICKTASK_H
