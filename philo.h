#ifndef PHILO_H
# define PHILO_H

//HEADERS
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#ifndef NUM_MEALS
# define NUM_MEALS 100
#endif

# define PHILO_MAX 200

//STRUCT
// argv[2] time to die  (in milliseconds) milliseconds since the beginning of their last meal or the beginning of the simulation calculate this 
//beginning+time to die  or last_meal_end+time_to_die
// argv[3] time to eat  (in milliseconds)
// argv[4] time to sleep  (in milliseconds)

//this stricture contains info about each philisopher
typedef struct s_philo
{
    pthread_t		thread;
	int				philo_id;
	int				eating; //1 0
	int				meals_eaten; //++
	size_t			last_meal; //start eating + time to eat
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				num_of_philos;
	int				num_meals;
	int				*dead;
	// pthread_mutex_t	*r_fork;
	// pthread_mutex_t	*l_fork;
	// pthread_mutex_t	*write_lock;
	// pthread_mutex_t	*dead_lock;
	// pthread_mutex_t	*meal_lock;
}   t_philo;

typedef struct s_program
{
	// int				dead_flag;
	// pthread_mutex_t	dead_lock;
	// pthread_mutex_t	meal_lock;
	// pthread_mutex_t	write_lock;
	t_philo			*philos;
}					t_program;


//helper functions check input
void ft_putstr_fd(char *str, int fd);
int input_ok(int argc, char **argv);
void	init_input(t_philo *philo, char **argv);

//INIT
void	init_input(t_philo *philo, char **argv);

//UTILS
size_t	get_current_time(void);
int ft_atoi(char *str);

#endif