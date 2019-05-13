#include "p2_threads.h"
#include "utils.h"

extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;

void *threadfunc(void *parm)
{

    int status;
    printf(" [Thread] Start\n");

    printf(" [Thread] Locks\n");
    status = pthread_mutex_lock(&mutex);

    printf(" [Thread] Blocked\n");
    status = pthread_cond_wait(&cond, &mutex);

    printf(" [Thread] Starts again.\n");
    for (int i=0; i<3; i++) {
        printf(" [Thread] Complete thread after (%d) seconds\n", (3-i));
        usleep(MSEC(1000));
    }

    printf(" [Thread] Unlocks\n");
    status = pthread_mutex_unlock(&mutex);
    printf(" [Thread] Complete\n");
    
    pthread_exit(NULL);
}



void *inputthread(void *parm)
{
    thread_arg *info = (thread_arg *)parm;
    int status;
    int gender;

    // Generate Person
    while (info->man_cnt > 0 || info->woman_cnt > 0)
    {
        if (info->man_cnt > 0 && info->woman_cnt > 0)
            gender = rand() % 2;
        else if (info->man_cnt == 0 && info->woman_cnt > 0)
            gender = 1;
        else // if (info->woman_cnt == 0 && info->man_cnt > 0)
            gender = 0;

        status = pthread_mutex_lock(&mutex);
        if (gender)
            info->woman_cnt--;
        else
            info->man_cnt--;

        Person p;
        p.set_gender(gender);
        //p.set_order(info->total_ppl - (info->man_cnt + info->woman_cnt));
        int wait_time = rand() % 8 + 1;
        p.set_time(wait_time + 2);

        info->wait_queue.push_back(p);
        status = pthread_mutex_unlock(&mutex);
        print_input(*info, gender); 

        int time = rand() % 5 + 1;
        usleep(MSEC(time));
    }
    //printf(" [Input] Complete\n");
    pthread_exit(NULL);
}

void woman_wants_to_enter(thread_arg *t)
{
    int i = 0;
    bool available = true;
    while (t->wait_queue[i].get_gender() != 1)
    {   
        i++;
        if (i == t->wait_queue.size())
        {
            available = false;
            break;
        }
    }

    if (available)
    {
        long time_to_stay = t->wait_queue[i].get_time();
        pthread_mutex_lock(&mutex);
        t->rooms.add_person(t->wait_queue[i]);
        t->wait_queue.erase(t->wait_queue.begin()+i);
        pthread_mutex_unlock(&mutex);

        struct timeval t_curr;
        gettimeofday(&t_curr, NULL);
        long mtime = get_elasped_time(t->t_start, t_curr);
        room_status rmst = t->rooms.get_room_stts();
        printf(" [%li ms][Queue] Send (Woman) into the fitting room (Stay %li ms), Status: Total: %d (Men: %d, Women: %d)\n", 
            mtime, time_to_stay, rmst.total, rmst.man, rmst.woman);

        const char * stts = "WomenPresent";
        if (t->rooms.get_status() == FULL)
            stts = "Full";
        printf(" [%li ms][fitting room] (Woman) goes into the fitting room, State is (%s): Total: %d (Men: %d, Women: %d)\n", 
            mtime, stts, rmst.total, rmst.man, rmst.woman);
    }
}
void man_wants_to_enter(thread_arg *t)
{
    int i = 0;
    bool available = true;
    while (t->wait_queue[i].get_gender() != 0)
    {
        i++;
        if (i == t->wait_queue.size())
        {
            available = false;
            break;
        }
    }

    if (available)
    {
        long time_to_stay = t->wait_queue[i].get_time();
        pthread_mutex_lock(&mutex);
        t->rooms.add_person(t->wait_queue[i]);
        t->wait_queue.erase(t->wait_queue.begin()+i);
        pthread_mutex_unlock(&mutex);
        
        struct timeval t_curr;
        gettimeofday(&t_curr, NULL);
        long mtime = get_elasped_time(t->t_start, t_curr);
        room_status rmst = t->rooms.get_room_stts();
        printf(" [%li ms][Queue] Send (Man) into the Fitting room (Stay %li ms), Status: Total: %d (Men: %d, Women: %d)\n", 
            mtime, time_to_stay, rmst.total, rmst.man, rmst.woman);

        const char * stts = "MenPresent";
        if (t->rooms.get_status() == FULL)
            stts = "Full";
        printf(" [%li ms][fitting room] (Man) goes into the fitting room, State is (%s): Total: %d (Men: %d, Women: %d)\n", 
            mtime, stts, rmst.total, rmst.man, rmst.woman);
    }
}

void *queuethread(void *parm)
{
    thread_arg *info = (thread_arg *)parm;

    while (info->man_cnt > 0 || !info->wait_queue.empty() || info->woman_cnt > 0)
    {
        if (!info->wait_queue.empty())
        {
            switch (info->rooms.get_status())
            {
                case 0:
                    if (info->wait_queue[0].get_gender())
                        woman_wants_to_enter(info);
                    else
                        man_wants_to_enter(info);
                    break;
                case 1:
                    woman_wants_to_enter(info);
                    break;
                case 2:
                    man_wants_to_enter(info);
                    break;
                default:
                    break;
            }
        }
    }

    //printf(" [Queue] Complete\n");
    pthread_exit(NULL);
}

void woman_leaves(thread_arg *t, int i)
{
    pthread_mutex_lock(&mutex);
    t->rooms.remove_person(i);
    pthread_mutex_unlock(&mutex);

    struct timeval t_curr;
    gettimeofday(&t_curr, NULL);
    room_status rmst = t->rooms.get_room_stts();

    const char * stts = "WomenPresent";
    if (t->rooms.get_status() == EMPTY)
        stts = "Empty";
    printf(" [%li ms][fitting room] (Woman) left the fitting room. Status is changed, Status is (%s): Total: %d (Men: %d, Women: %d)\n", 
        get_elasped_time(t->t_start, t_curr), stts, rmst.total, rmst.man, rmst.woman);
}
void man_leaves(thread_arg *t, int i)
{
    pthread_mutex_lock(&mutex);
    t->rooms.remove_person(i);
    pthread_mutex_unlock(&mutex);

    struct timeval t_curr;
    gettimeofday(&t_curr, NULL);
    room_status rmst = t->rooms.get_room_stts();

    const char * stts = "MenPresent";
    if (t->rooms.get_status() == EMPTY)
        stts = "Empty";
    printf(" [%li ms][fitting room] (Man) left the fitting room. Status is changed, Status is (%s): Total: %d (Men: %d, Women: %d)\n", 
        get_elasped_time(t->t_start, t_curr), stts, rmst.total, rmst.man, rmst.woman);
}

void *roomthread(void *parm)
{
    thread_arg *info = (thread_arg *)parm;
    int status;
    
    int cnt = info->total_ppl;
    while (cnt > 0)
    {
        for (int i=0; i<info->rooms.get_room_stts().total; i++)
        {
            if (info->rooms.ready_to_leave(i))
            {
                if (info->rooms.get_gender(i))
                    woman_leaves(info, i);
                else
                    man_leaves(info, i);
                cnt--;
            }
        }
    }

    //printf(" [Fittingroom] Complete\n");
    pthread_exit(NULL);
}

void print_input(thread_arg t, int gender)
{
    struct timeval t_curr;
    gettimeofday(&t_curr, NULL);
    if (gender)
        printf(" [%li ms][Input] A person (Woman) goes into the queue\n", get_elasped_time(t.t_start, t_curr));
    else
        printf(" [%li ms][Input] A person (Man) goes into the queue\n", get_elasped_time(t.t_start, t_curr));
}

void thread_main(int num_gender, int num_room)
{
    thread_arg global_info;
    global_info.total_ppl = num_gender * 2;
    global_info.man_cnt = num_gender;
    global_info.woman_cnt = num_gender;
    global_info.rooms.set_size(num_room);
    gettimeofday(&global_info.t_start, NULL);

    // Threads Creation
    pthread_t i_tid = 0;
    pthread_t q_tid = 0;
    pthread_t r_tid = 0;
    
    if (pthread_create(&q_tid, NULL, queuethread, (void *)&global_info))
    {
        fprintf(stderr, "Error creating queue_thread\n");   
    }
    if (pthread_create(&r_tid, NULL, roomthread, (void *)&global_info))
    {
        fprintf(stderr, "Error creating room_thread\n");   
    }
    if (pthread_create(&i_tid, NULL, inputthread, (void *)&global_info))
    {
        fprintf(stderr, "Error creating input_thread\n");   
    }


    // Threads Finish
    if(pthread_join(i_tid, NULL))
    {
        fprintf(stderr, "Error joining input_thread\n");  
    }
    if(pthread_join(q_tid, NULL))
    {
        fprintf(stderr, "Error joining queue_thread\n");  
    }
    if(pthread_join(r_tid, NULL))
    {
        fprintf(stderr, "Error joining room_thread\n");  
    }

}