#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

#define MAX_JOBS 100

typedef struct
{
    int task_id;
    int release_at;
    int deadline;
    int remaining_units;
    int deadline_missed;
    int completed;
} Job;

void sort_tasks_by_period(Task tasks[], int task_count);

void generate_jobs(Task tasks[], int task_count, Job jobs[], int *job_count, int current_time);

int select_rms_job(Job jobs[], int job_count, Task tasks[], int task_count, int current_time);

void check_deadlines(Job jobs[], int job_count, int current_time, int *deadline_misses);

#endif
