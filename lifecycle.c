#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died


static void safe_print(t_philo *philo, char *str)
{
    pthread_mutex_lock(philo->ptr_dead_lock);  // Lock the mutex for accessing dead_flag
    if (*philo->ptr_dead_flag == 0) 
    {
        pthread_mutex_unlock(philo->ptr_dead_lock);  // Unlock the mutex if no dead flag
        pthread_mutex_lock(philo->ptr_write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d %s\n", get_current_time(), philo->philo_id, str);
        pthread_mutex_unlock(philo->ptr_write_lock);  // Unlock the write mutex after printing
    }
    pthread_mutex_unlock(philo->ptr_dead_lock);  // Unlock the mutex if dead flag is set
}

static void print_death(t_philo *philo)
{
    pthread_mutex_lock(philo->ptr_dead_lock);  // Lock the mutex for accessing dead_flag
    if (*philo->ptr_dead_flag == 1) 
    {
        pthread_mutex_unlock(philo->ptr_dead_lock);  // Unlock the mutex if no dead flag
        pthread_mutex_lock(philo->ptr_write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d has died\n", get_current_time(), philo->philo_id);
        pthread_mutex_unlock(philo->ptr_write_lock);  // Unlock the write mutex after printing
    }
    pthread_mutex_unlock(philo->ptr_dead_lock);  // Unlock the mutex if dead flag is set
}

static void think(t_philo philo)
{
    safe_print(&philo, " is thinking");
     
} 

static void ft_sleep(t_philo philo)
{
    safe_print(&philo, " is sleeping");
    usleep(philo.time_to_sleep * 1000);
}

//set DEAD FLAG to 0 when last_meal + time_to_die < current time && eating == 0
//or
//all philos have all_ate == 1 and num_meals != -1;
static void eat(t_philo *philo)
{
    if (philo->philo_id % 2 == 0)
	    usleep(1);
    // Lock the right fork
    pthread_mutex_lock(philo->r_fork);//
    safe_print(philo, " has taken right fork");
    //Lock the left fork
    pthread_mutex_lock(philo->l_fork);//
    safe_print(philo, " has taken left fork");
    //increase meals counter
    safe_print(philo, " is eating");
    pthread_mutex_lock(philo->ptr_meal_lock);//
    philo->meals_counter++;
    philo->eating = 1;
    pthread_mutex_unlock(philo->ptr_meal_lock);
    //put the forks down
    pthread_mutex_unlock(philo->l_fork);//
    pthread_mutex_unlock(philo->r_fork);//
    
    pthread_mutex_lock(philo->ptr_meal_lock);
    if (philo->num_meals != -1 && philo->meals_counter >= philo->num_meals)
    {
        pthread_mutex_lock(philo->ptr_dead_lock);
        *philo->ptr_dead_flag = 1;        ////here is a problem, whe one of philos has eaten n times it quits, i need to check it for
        pthread_mutex_unlock(philo->ptr_dead_lock);
    }
    usleep(philo->time_to_eat * 1000);
    pthread_mutex_unlock(philo->ptr_meal_lock);

}

//function executed in each tread
void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
   
    if (philo->num_of_philos == 1)
    {
        usleep(philo->time_to_die * 1000);
        pthread_mutex_lock(philo->ptr_dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(philo->ptr_dead_lock);
        print_death(philo);
        return arg;
    }
    

    while (*philo->ptr_dead_flag != 1)
    {
        eat(philo);

        ft_sleep(*philo);
        think(*philo);
    }
 
    printf("Hello from philo %d, my left fork is %p and right forkis %p\n", philo->philo_id, philo->l_fork, philo->r_fork);
    printf("Dead flag %d\n", *(philo->ptr_dead_flag));
    return arg;
}