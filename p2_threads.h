#ifndef __P2_THREADS_H
#define __P2_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include "types_p2.h"

typedef struct {
    int total_ppl;
    int man_cnt;
    int woman_cnt;
    struct timeval t_start;
    vector<Person> wait_queue;
    Fittingroom rooms;
} thread_arg;

void *threadfunc(void *parm);
void *queuethread(void *parm);
void *roomthread(void *parm);
void woman_wants_to_enter(thread_arg *t);
void man_wants_to_enter(thread_arg *t);
void woman_leaves(thread_arg *t, int i);
void man_leaves(thread_arg *t, int i);
void print_input(thread_arg t, int gender);
void thread_main(int num_gender, int num_room);


#endif
