#ifndef PHILO_H
#define PHILO_H

//HEADERS
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

//STRUCT
// argv[2] time to die  (in milliseconds)
// argv[3] time to eat  (in milliseconds)
// argv[4] time to sleep  (in milliseconds)
typedef struct s_philo
{

    //pthread_t thread[num_philo]; //???how to define arr of threads? thread[4] for ex?
    int num_philo;
    int num_forks;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_time_to_eat;

}   t_philo;



//helper functions check input
int is_digit(char *str);
void ft_putstr_fd(char *str, int fd);
void handle_input(t_philo *data, char **argv);
#endif