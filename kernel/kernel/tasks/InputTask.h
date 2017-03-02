//
// Created by Tangruiwen on 2017/2/25.
//

#ifndef MOONOS_INPUTTASK_H_H
#define MOONOS_INPUTTASK_H_H

#include <kernel/tasks/task.h>

#ifdef __cplusplus
class InputTask : public AbstractTask{
public:
    InputTask();
    virtual ~InputTask();
    void run() override ;

private:
    void do_input_read();
    void do_input_write();
    void in_process(uint32_t key);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

void input_task_main();

#ifdef __cplusplus
}
#endif

#endif //MOONOS_INPUTTASK_H_H
