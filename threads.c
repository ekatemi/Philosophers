#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

//ROUTINES

// void *eat_routine(t_philo data)
// {
//     //mutex/
//     usleep(data.time_to_eat);
//     //data.time_to_die = 
//     //end mutex
// }

// void *sleep_routine(t_philo data)
// {
//     //mutex/
//     usleep(data.time_to_sleep);
//     //end mutex
// }


void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    printf("Hello from philo %d\n", philo->philo_id);
    return NULL;
}