/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_INTERVALS 1000

// Structure to store time intervals
typedef struct {
    int start;
    int end;
} Interval;

int cmp(const void *a, const void *b)
{
    return ((Interval*)a)->start - ((Interval*)b)->start;
}
// Function to find idle time intervals
int findIdleIntervals(Interval activeIntervals[], int n, int startTime, int endTime, Interval idleIntervals[]) {
    // TODO: Implement sorting and merging logic to compute idle intervals
    qsort(activeIntervals, n, sizeof(Interval), cmp );
    
    Interval *merged = (Interval *)malloc(sizeof(Interval) * n);
    
    merged[0].start = activeIntervals[0].start;
    merged[0].end = activeIntervals[0].end;
    int index = 0;
    for(int i=1;i<n;i++)
    {
        if(merged[index].end >activeIntervals[i].start)
        {
            merged[index].end = fmax(merged[index].end, activeIntervals[i].end);
        }
        else
        {
            index++;
            merged[index].start = activeIntervals[i].start;
            merged[index].end = activeIntervals[i].end;
        }
    }
    
   /* for(int i=0;i<index+1;i++)
    {
        printf("%d %d\n",merged[i].start,merged[i].end);
    }*/
    int count = 0;
    // beginning condition
    if(merged[0].start > startTime)
    {
        count++;
        printf("%d %d\n",merged[0].start,startTime);
    }
    
    int diff = 0;
    for(int i=1;i<index+1;i++)
    {
        diff = merged[i].start - merged[i-1].end;
        if(diff > 0)
        {
            count++;
            printf("%d %d\n",merged[i].start,merged[i-1].end);
        }
    }
    
    //end case
    if(merged[index].end < endTime)
    {
        printf("end case %d %d\n",merged[index].end,endTime);
        count++;
    }
    return count; // Return the number of idle intervals found
}

int main() {
    // Define observation window
    int startTime = 0, endTime = 120;
    
    // Define active intervals
    Interval activeIntervals[] = {
        {10, 30}, {20, 70}, {90, 120}
    };
    int n = sizeof(activeIntervals) / sizeof(activeIntervals[0]);

    // Array to store idle intervals
    Interval idleIntervals[MAX_INTERVALS];
    
    // Find idle intervals
    int idleCount = findIdleIntervals(activeIntervals, n, startTime, endTime, idleIntervals);
    printf("%d\n", idleCount);
    // Print idle intervals
    printf("Idle Intervals:\n");
    for (int i = 0; i < idleCount; i++) {
        printf("[%d, %d]\n", idleIntervals[i].start, idleIntervals[i].end);
    }

    return 0;
}


