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

//prints death, so dead flag should be set to 1 before or after
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
} 

//print only if dead flag is 0
static void ft_sleep(t_philo *philo)
{
    
    safe_print(philo, " is sleeping");
    ft_usleep(philo->time_to_sleep);
}

//print only if dead flag is 0
static void eat(t_philo *philo)
{
    // Lock the right fork
    pthread_mutex_lock(philo->r_fork);
    safe_print(philo, " has taken right fork");

    // Lock the left fork
    pthread_mutex_lock(philo->l_fork);
    safe_print(philo, " has taken left fork");

    // Indicate that the philosopher is eating
    safe_print(philo, " is eating");

    // Update the last meal time
    pthread_mutex_lock(&philo->program->meal_lock);
    philo->last_meal = get_current_time();
    philo->meals_counter++;
    pthread_mutex_unlock(&philo->program->meal_lock);

    // Check if all meals have been consumed
    if (philo->num_meals != -1 && philo->meals_counter == philo->num_meals)
    {
        pthread_mutex_lock(&philo->program->meal_lock);
        philo->program->finished_philo_counter++;
        philo->eating = 0;
        printf("philos finished meals------------------- %d\n", philo->program->finished_philo_counter);
        pthread_mutex_unlock(&philo->program->meal_lock);
    }

        // Sleep for the eating duration
    
    // Release the left fork
    pthread_mutex_unlock(philo->l_fork);
    
    // Release the right fork
    pthread_mutex_unlock(philo->r_fork);
    ft_usleep(philo->time_to_eat);
}


//function executed in each tread
void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (philo->philo_id % 2 == 0)
        ft_usleep(philo->time_to_eat);

    if (philo->num_of_philos == 1)
    {
        ft_usleep(philo->time_to_die);
        //print_death(philo);
        pthread_mutex_lock(&philo->program->dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(&philo->program->dead_lock);
        
        return arg;
    }
    //end all threads id dead flag == 1
    while (!check_dead_flag(philo))
    {
        eat(philo);    
        ft_sleep(philo);
        think(philo);  
    }
    return arg;
}



