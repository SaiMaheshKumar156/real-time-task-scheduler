#ifndef INPUT_H
#define INPUT_H

#include "task.h"

#define MAX_TASKS 100

int load_tasks_from_csv(const char *filename, Task tasks[], int max_tasks);

#endif
