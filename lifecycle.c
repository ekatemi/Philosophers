#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died


static void safe_print(t_philo philo, char *str)
{
    pthread_mutex_lock(philo.ptr_write_lock);
    printf("%zu philo id %d %s\n", get_current_time(), philo.philo_id, str);
    pthread_mutex_unlock(philo.ptr_write_lock);
}

static void think(t_philo philo)
{
    safe_print(philo, " is thinking");
} 

static void ft_sleep(t_philo philo)
{
    safe_print(philo, " is sleeping");
    usleep(philo.time_to_sleep * 1000);
}

static void eat(t_philo philo)
{
    pthread_mutex_lock(philo.r_fork);
	safe_print(philo, " has taken a fork");
	if (philo.num_of_philos == 1)
	{
		usleep(philo.time_to_die * 1000);
		pthread_mutex_unlock(philo.r_fork);
		return ;
	}
	pthread_mutex_lock(philo.l_fork);
	safe_print(philo, " has taken a fork");
	philo.eating = 1;
	safe_print(philo, " is eating");
	pthread_mutex_lock(philo.ptr_meal_lock);
	philo.last_meal = get_current_time();
	philo.meals_eaten++;
	pthread_mutex_unlock(philo.ptr_meal_lock);
	usleep(philo.time_to_eat * 1000);
	philo.eating = 0;
	pthread_mutex_unlock(philo.l_fork);
	pthread_mutex_unlock(philo.r_fork);
}

//function executed in each tread
void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    //if id even - wait 1 ms
    if (philo->philo_id % 2 == 0)
	    usleep(1);
    
    while (*philo->ptr_dead_flag != 1)
    {
        eat(*philo);
        ft_sleep(*philo);
        think(*philo);
    }
    
    printf("Hello from philo %d, my left fork is %p and right forkis %p\n", philo->philo_id, philo->l_fork, philo->r_fork);
    printf("Dead flag %d\n", *(philo->ptr_dead_flag));
    return arg;
}