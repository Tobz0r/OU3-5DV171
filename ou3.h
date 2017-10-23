#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
typedef struct thread_time_ops{
        double create_time; /* start time for program*/
        double start_w_time; /*start workign time*/
        double end_w_time; /*end time for work*/
        double end_time; /* end time for program*/
        int id; /*id for file*/
}thread_time_ops;
/*check to see if args are correct*/
bool isNumber(char number[]);
/* benchmark that creates files and reads/writes to them*/
void *benchmark(void* temp);
/*returns current time in miliseconds*/
double getRealTime();
