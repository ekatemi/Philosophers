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


void *routine()
{
    printf("Hello from philo");
    // t_philo *data = (t_philo *)arg;
    // data->start_time = get_current_time();
    // printf("time: %ld philosopher is eating", get_current_time());
    // //how long it is eating? (start + data.time_to_eat) * 1000;
    // data->meals_eaten++;
    // usleep((data->start_time + data->time_to_eat) * 1000);
    
    // data->start_time = get_current_time();//it is also last meal
    // printf("time: %ld philosopher is sleeping", get_current_time());
    // //usleep((data->start_time + data->time_to_sleep) * 1000) == -1;
    
    // printf("time: %ld philosopher is thinking", get_current_time());
    // while (get_current_time() - data->start_time < data->time_to_die)
    // {
    //     usleep(500);
    //     get_current_time();
    // }
    return NULL;
}