//
// Created by Tangruiwen on 2017/2/25.
//

#ifndef MOONOS_TASK_H
#define MOONOS_TASK_H
#include <stdint.h>

#define INVALID_DRIVER	-20
#define INTERRUPT	-10
#define TASK_INPUT 	0
#define TASK_HD		1
#define TASK_TICK   2
#define TASK_FS 	3
#define ANY		0x0fffffff - 2
#define NO_TASK		0x0fffffff - 1

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*task_main)();

struct _task {
    task_main entry_point;
    uint32_t esp_size;
    char name[32];
    uint32_t privilege;
};

typedef struct _task TASK_T;


#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
class AbstractTask{
public:
    AbstractTask() {}
    virtual ~AbstractTask() {}
    virtual void run() = 0;
};

#endif

#endif //MOONOS_TASK_H
