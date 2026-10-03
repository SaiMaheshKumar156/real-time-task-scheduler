# Real-Time Task Scheduler Simulator

A C-based simulation of **Rate Monotonic Scheduling (RMS)** for periodic real-time tasks.

## Project Overview

This project implements a discrete-time simulation of a single-processor real-time scheduler using the **Rate Monotonic Scheduling (RMS)** algorithm.

The simulator works with periodic tasks defined by their execution time, period, and deadline. At each simulation time step, new jobs are generated when their corresponding tasks are released. The scheduler then selects the highest-priority ready job based on the task period and executes it for one time unit.

The simulator tracks job execution and deadline status throughout the simulation and produces an execution timeline and scheduling summary at the end.

The project also calculates the total task-set utilization and compares it with the theoretical RMS utilization bound.

## Features

* Reads periodic task definitions from a CSV file.
* Supports multiple tasks with configurable execution time, period, and deadline.
* Sorts tasks by period to establish Rate Monotonic priority order.
* Generates jobs periodically according to each task's period.
* Selects the highest-priority ready job using RMS scheduling.
* Simulates CPU execution in discrete time units.
* Tracks the remaining execution time of each job.
* Detects missed deadlines during simulation.
* Tracks completed, unfinished, and pending jobs.
* Generates a complete CPU execution timeline.
* Calculates CPU busy time and CPU idle time.
* Calculates actual CPU utilization.
* Calculates task-set utilization.
* Calculates the theoretical RMS utilization bound.
* Performs an RMS utilization-bound test.
* Provides a summary of the scheduling results after the simulation.

## Project Structure

Real Time Schedular/
│
├── data/
│   └── tasks.csv
│
├── include/
│   ├── task.h
│   ├── input.h
│   └── scheduler.h
│
├── src/
│   ├── main.c
│   ├── input.c
│   └── scheduler.c
│
└── README.md

### Directory and File Description

| Path                  | Description                                                                       |
| --------------------- | --------------------------------------------------------------------------------- |
| `data/tasks.csv`      | Contains the periodic task definitions used by the simulator                      |
| `include/task.h`      | Defines the `Task` structure                                                      |
| `include/input.h`     | Declares the CSV task-loading function and task limit                             |
| `include/scheduler.h` | Defines the `Job` structure and scheduler function declarations                   |
| `src/input.c`         | Reads and parses task data from the CSV file                                      |
| `src/scheduler.c`     | Implements task sorting, job generation, RMS job selection, and deadline checking |
| `src/main.c`          | Runs the simulation and generates the timeline and final statistics               |
| `README.md`           | Project documentation                                                             |

## Input Format

The simulator reads task definitions from:

data/tasks.csv

The CSV file uses the following format:

id,execution_time,period,deadline

Each row represents one periodic real-time task.

### Parameters

| Parameter        | Description                                    |
| ---------------- | ---------------------------------------------- |
| `id`             | Unique identifier for the task                 |
| `execution_time` | Number of CPU time units required by each job  |
| `period`         | Time interval between consecutive job releases |
| `deadline`       | Relative deadline of each job                  |

### Example

id,execution_time,period,deadline
1,1,4,4
2,1,5,5
3,1,8,8
4,1,12,12
5,1,20,20

For example, the task:

1,1,4,4

means:

* Task ID = `1`
* Execution time = `1` time unit
* Period = `4` time units
* Relative deadline = `4` time units

Therefore, jobs of Task 1 are released at:

0, 4, 8, 12, 16, ...

with each job requiring 1 CPU time unit.

## How the RMS Scheduler Works

The simulator uses **Rate Monotonic Scheduling (RMS)**, a fixed-priority scheduling algorithm for periodic real-time tasks.

In RMS:

> **A task with a shorter period has a higher priority.**

For example, consider these tasks:

Task 1: Execution = 1, Period = 4
Task 2: Execution = 1, Period = 6
Task 3: Execution = 1, Period = 10

Their RMS priority order is:

Higher Priority
      ↓
Task 1  → Period 4
Task 2  → Period 6
Task 3  → Period 10
      ↓
Lower Priority

### Scheduling Process

At each simulation time step, the scheduler performs the following operations:

1. **Check deadlines** of previously released jobs.
2. **Generate new jobs** for tasks whose period has been reached.
3. **Find ready jobs** whose release time has occurred and whose execution is not finished.
4. **Select the highest-priority job** according to RMS.
5. **Execute the selected job for one CPU time unit.**
6. **Update its remaining execution time.**
7. Mark the job as completed when its remaining execution time reaches zero.
8. Move to the next simulation time step.

### Example

Consider:

Task 1: Execution = 2, Period = 5
Task 2: Execution = 1, Period = 7

Task 1 has the shorter period, so it has higher RMS priority.

A simplified execution sequence could look like:

Time 0 → Task 1
Time 1 → Task 1
Time 2 → Task 2
Time 3 → IDLE
Time 4 → IDLE
Time 5 → Task 1
...

If a higher-priority task becomes ready while another lower-priority task still has remaining execution time, the scheduler can select the higher-priority task at the next simulation step.

### Job Model

Each released task instance is represented as a `Job` containing:

* Task ID
* Release time
* Absolute deadline
* Remaining execution units
* Deadline-missed status
* Completion status

This allows the simulator to track individual jobs rather than only tracking the original task definitions.

## Build and Run

### Prerequisites

You need:

* A C compiler such as **GCC**
* A terminal such as PowerShell, Command Prompt, or a Unix-like shell
* The project source files and `data/tasks.csv`

### Compile

From the project root directory, run:

```powershell
gcc -Wall -Wextra -Iinclude src\main.c src\input.c src\scheduler.c -o scheduler.exe
```

The compiler options used are:

* `-Wall` — enables common compiler warnings
* `-Wextra` — enables additional compiler warnings
* `-Iinclude` — tells GCC where to find the project header files
* `-o scheduler.exe` — creates the executable named `scheduler.exe`

### Run

After compilation:

```powershell
.\scheduler.exe
```

The program reads the task definitions from:

data/tasks.csv

and runs the RMS simulation for the configured simulation duration.

The program then displays:

* Task information
* Job execution messages
* Execution timeline
* Job completion status
* Deadline misses
* CPU utilization
* Task-set utilization
* RMS utilization bound
* RMS bound test result

## Example Output

For the following task set:

Task 1: Execution=1, Period=4, Deadline=4
Task 2: Execution=1, Period=5, Deadline=5
Task 3: Execution=1, Period=8, Deadline=8
Task 4: Execution=1, Period=12, Deadline=12
Task 5: Execution=1, Period=20, Deadline=20

the simulator produced the following summary:

========== RMS Simulation Summary ==========

Simulation Time                 : 50
Jobs Generated                  : 34

Jobs Completed On Time          : 34
Jobs Completed After Deadline   : 0
Unfinished Jobs                 : 0
Pending at Simulation End       : 0

Deadline Misses                 : 0

CPU Busy Time                   : 34
CPU Idle Time                   : 16
Actual CPU Utilization          : 68.00%

Task Set Utilization            : 63.33%
RMS Utilization Bound           : 74.35%
RMS Bound Test                  : PASS
============================================

The execution timeline generated by the simulator shows which job was executed at each simulation time step, including periods where the CPU was idle.

## RMS Utilization Bound

The simulator calculates the theoretical utilization bound for Rate Monotonic Scheduling using the Liu-Layland bound:

[
U_{RMS} = n(2^{1/n} - 1)
]

where:

* `n` is the number of tasks.
* `U_RMS` is the theoretical utilization bound.

The task-set utilization is calculated as:

[
U = \sum_{i=1}^{n}\frac{C_i}{T_i}
]

where:

* `C_i` is the execution time of task `i`.
* `T_i` is the period of task `i`.

### Interpretation

If:

Task Set Utilization <= RMS Utilization Bound

the simulator reports:

RMS Bound Test : PASS

A PASS means the task-set utilization is below the theoretical RMS utilization bound, which is a sufficient schedulability condition under the assumptions of the Liu-Layland bound.

If:

Task Set Utilization > RMS Utilization Bound

the simulator reports:

RMS Bound Test : INCONCLUSIVE

`INCONCLUSIVE` does **not** by itself mean that the task set is unschedulable. It means that the utilization-bound test cannot guarantee schedulability for that task set. The actual simulation results, including deadline misses and unfinished jobs, provide additional information about the simulated schedule.

For example, in the sample run:

Task Set Utilization     : 63.33%
RMS Utilization Bound    : 74.35%
RMS Bound Test           : PASS

## Limitations and Future Improvements

The current version focuses on demonstrating the core concepts of a Rate Monotonic Scheduler through a discrete-time simulation.

### Current Limitations

* Simulation time is currently configured directly in the source code.
* The simulator uses a fixed-size job array.
* The CSV loader currently skips the first line as a header.
* The current scheduler implements RMS priority selection only.
* The simulation uses integer time units.
* The current implementation does not provide a graphical visualization of the schedule.
* The RMS utilization-bound calculation is a sufficient test, not a complete schedulability analysis.

### Future Improvements

Possible extensions include:

* Add configurable simulation time through user input or a configuration file.
* Implement **Earliest Deadline First (EDF)** scheduling.
* Add more detailed job statistics and per-task performance metrics.
* Improve input validation and error reporting.
* Add graphical or Gantt-chart visualization of the execution timeline.
* Add automated test cases for different task sets.
* Support larger simulations and configurable job limits.
* Improve the scheduler architecture to make additional scheduling algorithms easier to integrate.

