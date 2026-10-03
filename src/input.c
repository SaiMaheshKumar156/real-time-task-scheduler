#include <stdio.h>
#include "input.h"

int load_tasks_from_csv(const char *filename, Task tasks[], int max_tasks)
{    
    FILE *file;
    char line[256];
    int count = 0;

    file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error: Could not open file.\n");
        return -1;
    }
    if (fgets(line, sizeof(line), file) ==NULL)
    {
        fclose(file);
        return 0;
    }
    while (fgets(line, sizeof(line), file)!=NULL && count < max_tasks)
    {
        if (sscanf(line, "%d,%d,%d,%d", 
            &tasks[count].id, 
            &tasks[count].execution_time, 
            &tasks[count].period, 
            &tasks[count].deadline)==4)
        {
            count++;
        }
    }

    fclose(file);

    return count;

}