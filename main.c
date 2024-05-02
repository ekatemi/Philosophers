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





int main (int argc, char **argv)
{
    int i;
    t_philo data;
    i = 1;
    if (argc > 4)
    {
        while (i < argc)
        {
            if (!is_digit(&argv[i][0]))
            {
                ft_putstr_fd("Wrong input", 2);
                return (0);
            }
            else if (i > 5)
            {
                ft_putstr_fd("Should be only 5 arguments: <num philosophers> <die> <eat> <sleep> <meals>", 2);
                return (0);
            }
            handle_input(&data, argv);
            i++;
        }
    }
    else 
    {
        ft_putstr_fd("Short input", 2);
    }
    printf("num philo %d\ntime to die %d\ntime to eat %d\ntime to sleep %d\noptional %d\n", data.num_philo, data.time_to_die, data.time_to_eat, data.time_to_sleep, data.num_time_to_eat);
    return (0);
}