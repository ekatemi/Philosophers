#include "philo.h"



//ROUTONES

void *eat_routine(t_philo data)
{
    //mutex/
    usleep(data.time_to_eat);
    //data.time_to_die = 
    //end mutex
}

void *sleep_routine(t_philo data)
{
    //mutex/
    usleep(data.time_to_sleep);
    //end mutex
}