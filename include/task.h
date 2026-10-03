#ifndef TASK_H
#define TASK_H

typedef struct
{
    int id;
    int execution_time;
    int period;
    int deadline;
} Task;

#endif