/* memset, printf, malloc, free, write,
usleep, gettimeofday, pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock

ARGUMENTS
number_of_philosophers time_to_die time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]

The specific rules for the mandatory part are:
• Each philosopher should be a thread.
• There is one fork between each pair of philosophers. Therefore, if there are several
philosophers, each philosopher has a fork on their left side and a fork on their right
side. If there is only one philosopher, there should be only one fork on the table.
• To prevent philosophers from duplicating forks, you should protect the forks state
with a mutex for each of them.*/

#include "philo.h"

// argv[1] num of philosophers == num of forks is from 1 to num
// argv[2] time to die  (in milliseconds)
// argv[3] time to eat  (in milliseconds)
// argv[4] time to sleep  (in milliseconds)
// argv[5] arr number of time each philo must eat OPTIONAL

// void *monitor()
// {
//     printf("Im a monitor\n");
//     return NULL;
// }

int main (int argc, char **argv)
{
    //idata holds data from input
    t_philo data;
    t_program set;
    if (!input_ok(argc, argv))
        return(1);
    printf("START TIME\n%zu\n", get_current_time());
    init_input(&data, argv);
    printf("INPUT: num philo %d\ntime to die %zu\ntime to eat %zu\ntime to sleep %zu\noptional %d\n", data.num_of_philos, data.time_to_die, data.time_to_eat, data.time_to_sleep, data.num_meals);
    init_mutexes(&set, &data);
    set_philosophers(&data, &set);
    create_and_join_threads(&data, &set);
    cleanup_all(&set);
    return (0);
}