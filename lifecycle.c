#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

//1 on, 0 off
int check_dead_flag(t_philo *philo)
{
    pthread_mutex_lock(&philo->program->dead_lock);
    if (*philo->ptr_dead_flag == 1)
        {
            pthread_mutex_unlock(&philo->program->dead_lock);
            return 1;
        }
    pthread_mutex_unlock(&philo->program->dead_lock);
    return 0;
}

//does not print after death flag set
void safe_print(t_philo *philo, char *str)
{
    if (!check_dead_flag(philo)) 
    {
        pthread_mutex_lock(&philo->program->write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d %s\n", get_current_time() - philo->start_time, philo->philo_id, str);
        pthread_mutex_unlock(&philo->program->write_lock);  // Unlock the write mutex after printing
    }
}

//only prints death, so dead flag should be set to 1 before or after
void print_death(t_philo *philo)
{    
        pthread_mutex_lock(&philo->program->write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d has died\n", get_current_time() - philo->start_time, philo->philo_id);
        pthread_mutex_unlock(&philo->program->write_lock);  // Unlock the write mutex after printing
}

//print only if dead flag is 0
static void think(t_philo *philo)
{
    safe_print(philo, " is thinking");
    
    //printf eating status
    pthread_mutex_lock(&philo->program->write_lock);
    printf("philo %d -> eating  %d\n",philo->philo_id, philo->eating);
    pthread_mutex_unlock(&philo->program->write_lock);
} 

//print only if dead flag is 0
static void ft_sleep(t_philo *philo)
{
    pthread_mutex_lock(&philo->program->meal_lock);
    philo->eating = 0;
    pthread_mutex_unlock(&philo->program->meal_lock);
    
    safe_print(philo, " is sleeping");
    ft_usleep(philo->time_to_sleep);
    
    //printf eating status
    pthread_mutex_lock(&philo->program->write_lock);
    printf("philo %d -> eating  %d\n",philo->philo_id, philo->eating);
    pthread_mutex_unlock(&philo->program->write_lock);
}

//print only if dead flag is 0
static void eat(t_philo *philo)
{
    // Lock the right fork
if (philo->philo_id % 2 == 0) {
        pthread_mutex_lock(philo->r_fork);
        safe_print(philo, " has taken right fork");
        pthread_mutex_lock(philo->l_fork);
        safe_print(philo, " has taken left fork");
    } else {
        pthread_mutex_lock(philo->l_fork);
        safe_print(philo, " has taken left fork");
        pthread_mutex_lock(philo->r_fork);
        safe_print(philo, " has taken right fork");
    }

    // Indicate that the philosopher is eating
    safe_print(philo, " is eating");

    // Update the last meal time
    pthread_mutex_lock(&philo->program->meal_lock);
    philo->last_meal = get_current_time();
    philo->eating = 1;
    pthread_mutex_unlock(&philo->program->meal_lock);
    
    //update individual meals counter
    philo->meals_counter++;

    // Check if all meals have been consumed
    if (philo->num_meals != -1 && philo->meals_counter == philo->num_meals)
    {
        pthread_mutex_lock(&philo->program->meal_lock);
        philo->program->finished_philo_counter++;
        //printf counter
        printf("philos finished all meals------------------- %d\n", philo->program->finished_philo_counter);
        pthread_mutex_unlock(&philo->program->meal_lock);
    }

    // Sleep for the eating duration
    ft_usleep(philo->time_to_eat);
    
    //printf eating status
    pthread_mutex_lock(&philo->program->write_lock);
    printf("philo %d -> eating  %d\n",philo->philo_id, philo->eating);
    pthread_mutex_unlock(&philo->program->write_lock);
    
    // Release the left fork
    pthread_mutex_unlock(philo->r_fork);
    // Release the right fork
    pthread_mutex_unlock(philo->l_fork);
}


//function executed in each tread
void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    //all start in different time
    if (philo->philo_id % 2 == 0)
        ft_usleep(philo->time_to_eat);
    //case only one thread
    if (philo->num_of_philos == 1)
    {
        ft_usleep(philo->time_to_die);
        
        //set dead flag to stop routine (dont know how to set it only in monitor)
        pthread_mutex_lock(&philo->program->dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(&philo->program->dead_lock);
        return arg;
    }
    //break the loop if dead flag == 1
    while (1)
    {
        if (check_dead_flag(philo))
            break ;
        eat(philo);    
        ft_sleep(philo);
        think(philo);  
    }
    return arg;
}



