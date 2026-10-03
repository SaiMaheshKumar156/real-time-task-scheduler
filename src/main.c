#include <stdio.h>
#include <math.h>
#include "input.h"
#include "scheduler.h"

int main(void)
{
    Task tasks[MAX_TASKS];
    int task_count;
    
    Job jobs[MAX_JOBS];
    int job_count = 0; 
    int deadline_misses = 0;
    int unfinished_jobs = 0;
    int pending_at_end = 0;
    int completed_on_time = 0;
    int completed_late = 0;
    
    double utilization = 0.0;
    double rms_bound = 0.0;

    int simulation_time = 50;
    int current_time = 0;
    int execution_timeline[simulation_time];
    
    int cpu_busy_time = 0;
    int cpu_idle_time = 0;
    
    task_count = load_tasks_from_csv("data/tasks.csv", tasks, MAX_TASKS);

    if (task_count < 0)
    {
        printf("Failed to load tasks.\n");
        return 1;
    }

    sort_tasks_by_period(tasks, task_count);

    for (int i = 0; i < task_count; i++)
    {
        printf("Task %d: Execution=%d, Period=%d, Deadline=%d\n",
        tasks[i].id,
        tasks[i].execution_time,
        tasks[i].period, 
        tasks[i].deadline);
    }

    for (int i = 0; i < task_count; i++)
    {
        utilization += 
            (double)tasks[i].execution_time / tasks[i].period;
    }

    rms_bound = task_count * (pow(2.0, 1.0 / task_count) - 1.0);

    while(current_time < simulation_time)
    {
        printf("Time = %d\n",current_time);

        check_deadlines(jobs, job_count, current_time, &deadline_misses);
        generate_jobs(tasks, task_count, jobs, &job_count, current_time);
        int selected_job = select_rms_job(jobs, job_count, tasks, task_count, current_time);
        if (selected_job != -1)
        {
            execution_timeline[current_time] = selected_job;
            cpu_busy_time++;
        } 
        else
        {
            execution_timeline[current_time] = -1;
            cpu_idle_time++;
        }

        if (selected_job != -1)
        {
            jobs[selected_job].remaining_units--;

            if (jobs[selected_job].remaining_units == 0)
            {
                jobs[selected_job].completed = 1;

                printf("Job %d (Task %d) completed, Remaining = 0\n", 
                        selected_job, 
                        jobs[selected_job].task_id);
            }
            else 
            {
                printf("Running Job %d (Task %d), Remaining = %d\n", 
                        selected_job, 
                        jobs[selected_job].task_id, 
                        jobs[selected_job].remaining_units);
            }
        }

        current_time++;
    }

    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].completed == 1)
        {
            if (jobs[i].deadline_missed == 1)
            {
                completed_late++;
            }
            else
            {
                completed_on_time++;
            }
        }
        else
        {
            unfinished_jobs++;

            if (jobs[i].deadline > simulation_time)
            {
                pending_at_end++;
            }
        }
    }

    double actual_cpu_utilization = 
        ((double)cpu_busy_time / simulation_time) * 100.0;

    printf("\n========== Execution Timeline ==========\n");

    for (int time = 0; time < simulation_time; time++)
    {
        if (execution_timeline[time] == -1)
        {
            printf("Time %d : IDLE\n", time);
        }
        else
        {
            int job_id = execution_timeline[time];

            printf("Time %d : Job %d (Task %d)\n", 
                    time, 
                    execution_timeline[time], 
                    jobs[job_id].task_id);
        }
    }
    printf("\n========== RMS Simulation Summary ==========\n");

    printf("Simulation Time                 : %d\n", simulation_time);

    printf("Jobs Generated                  : %d\n", job_count);

    printf("\nJobs Completed On Time          : %d\n", completed_on_time);

    printf("Jobs Completed After Deadline   : %d\n", completed_late);

    printf("Unfinished Jobs                 : %d\n", unfinished_jobs);

    printf("Pending at Simulation End       : %d\n", pending_at_end);

    printf("\nDeadline Misses                 : %d\n", deadline_misses);

    printf("\nCPU Busy Time                   : %d\n", cpu_busy_time);

    printf("CPU Idle Time                   : %d\n", cpu_idle_time);

    printf("Actual CPU Utilization          : %.2f%%\n", actual_cpu_utilization);

    printf("\nTask Set Utilization            : %.2f%%\n", utilization * 100.0);

    printf("RMS Utilization Bound           : %.2f%%\n", rms_bound * 100.0);
    
    if (utilization <= rms_bound)
    {
        printf("RMS Bound Test                  : PASS\n");
    }
    else
    {
        printf("RMS Bound Test                  : INCONCLUSIVE\n");
    }

    printf("========================================\n");
   
    return 0;
}