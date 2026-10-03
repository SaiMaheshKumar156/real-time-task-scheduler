#include <stdio.h>
#include "scheduler.h"

void sort_tasks_by_period(Task tasks[], int task_count)
{
    Task temp;

    for (int i=0; i<task_count; i++)
    {
        for (int j=0; j<task_count-1; j++)
        {
            if (tasks[j].period >tasks[j+1].period)
            {
                 temp = tasks[j];
                 tasks[j] = tasks[j+1];
                 tasks[j+1] = temp;
            }
        }
    }      
}

void generate_jobs(Task tasks[], int task_count, 
                   Job jobs[], int *job_count, int current_time)
{
    for (int i = 0; i < task_count; i++)
    {
        if (current_time % tasks[i].period == 0)
        {
            if (*job_count >= MAX_JOBS)
            {
                printf("Error: Maximum number of jobs reached.\n");
                return;
            }

            jobs[*job_count].task_id = tasks[i].id;
            jobs[*job_count].release_at = current_time;
            jobs[*job_count].deadline = current_time + tasks[i].deadline;
            jobs[*job_count].remaining_units = tasks[i].execution_time;
            jobs[*job_count].deadline_missed = 0;
            jobs[*job_count].completed = 0;

            (*job_count)++;
        }
    }
}

int select_rms_job(Job jobs[], int job_count, 
                   Task tasks[], int task_count, int current_time)
{
    int selected_job = -1;
    int selected_period = 0;

    for (int i=0; i < job_count; i++)
    {
        if (jobs[i].release_at <= current_time && 
            jobs[i].remaining_units > 0)
        {
            for (int j = 0; j < task_count; j++)
            {
                if (tasks[j].id == jobs[i].task_id)
                {
                    if (selected_job == -1)
                    {
                        selected_job = i;
                        selected_period = tasks[j].period;
                    }
                    else if (tasks[j].period < selected_period)
                    {
                        selected_job = i;
                        selected_period = tasks[j].period;
                    }
                }
            }
        }
    }

    return selected_job;
}

void check_deadlines(Job jobs[], int job_count, 
                     int current_time, int *deadline_misses)
{
    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].deadline <= current_time && 
            jobs[i].remaining_units > 0 && 
            jobs[i].deadline_missed ==0)
        {
            printf("Deadline missed: Job %d (Task %d)\n", 
                    i, jobs[i].task_id);

            jobs[i].deadline_missed = 1;
            (*deadline_misses)++;
        }
    }
}
