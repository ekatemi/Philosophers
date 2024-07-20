#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

//ROUTINES

//if one philo have died it sets dead for all
//waits till all_ate (this also sets dead flag)
void *monitor(void *arg)
{
    t_program *set;
    set = (t_program *)arg;
    
    while(1)
    {
       pthread_mutex_lock(&set->dead_lock);
        if (set->dead_flag == 1) 
        {
            pthread_mutex_unlock(&set->dead_lock);
            printf("Monitor exited because dead_flag == 1\n");
            break;
        }
        else
            pthread_mutex_unlock(&set->dead_lock);
        usleep(100);
    }
    return (arg);
}