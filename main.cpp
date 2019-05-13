#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//struct timeval t_global_start;

int main(int argc, char** argv)
{
    int num_gender = 0;
    int num_room = 0;
    vector<Person> waiting_quene;

    if (argc == 3 && (atoi(argv[1]) > 0 && atoi(argv[2]) > 0))
    {
        num_gender = atoi(argv[1]);
        num_room = atoi(argv[2]);

        thread_main(num_gender, num_room);
/*    
        // This is to set the global start time
        gettimeofday(&t_global_start, NULL);


        pthread_t       tid = 0;
        int             status = 0;
        int             work = 0;


        
        // Example code for sleep and class usage
        Person p1;
        p1.set_order(1);

        usleep(MSEC(200));
        p1.start();


        usleep(MSEC(150));
        p1.complete();
        ///////////////////////////////////////////


        if(pthread_create(&tid, NULL, threadfunc, NULL)) {
            fprintf(stderr, "Error creating thread\n");     
        }
        usleep(MSEC(10));

        for (int i=0; i<5; i++) {
            printf("Wake up thread after (%d) seconds\n", (5-i));
            usleep(MSEC(1000));
        }

        printf("Wake up thread\n");
        status = pthread_cond_signal(&cond);
        


        // wait for the second thread to finish
        if(pthread_join(tid, NULL)) {
            fprintf(stderr, "Error joining thread\n");  
        }
*/
    }
    else
    {
        printf("[ERROR] Expecting 2 arguments with integral value greater than zero. Got %d.\n", argc-1);
        printf("[USAGE] p2_exec <each gender cnt> <number of stalls>\n");
    }

    return 0;
}

