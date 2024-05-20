#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died
// static size_t get_last_meal_time(t_philo *philo)
// {
//     size_t last_meal_time = 0;
//     pthread_mutex_lock(&philo->program->meal_lock);
//     last_meal_time = philo->last_meal;
//     pthread_mutex_unlock(&philo->program->meal_lock);
//     return last_meal_time;
// }
//ROUTINES
static int is_dead(t_philo *philo)
{
    size_t time = get_current_time();
    pthread_mutex_lock(&philo->program->meal_lock);
    if(philo->eating == 0 && (philo->last_meal + philo->time_to_die) < time)
    {
        pthread_mutex_unlock(&philo->program->meal_lock);
        print_death(philo);
        return 1;
    }
    pthread_mutex_unlock(&philo->program->meal_lock);
    return 0;
}


static int check_all_philos(t_program *program)
{
    int i = 0;
    
    if (program->philos[0].num_of_philos == 1)
    {
        ft_usleep(program->philos[0].time_to_die);
        print_death(&program->philos[i]);
        // pthread_mutex_lock(&program->dead_lock);
        // *philo->ptr_dead_flag = 1;
        // pthread_mutex_unlock(&program->dead_lock);
        return 1;
    }
    
    while (i < program->philos[1].num_of_philos)
    {
        if (is_dead(&program->philos[i]))
        {
            // pthread_mutex_lock(&program->dead_lock);
            // program->dead_flag = 1;
            // pthread_mutex_unlock(&program->dead_lock);
            return 1;
        }
            
        i++;
    }
    return 0;
}

static int finish_meal_counter(t_program *program)
{
    int counter = 0;
    pthread_mutex_lock(&program->meal_lock);
    counter = program->finished_philo_counter;
    pthread_mutex_unlock(&program->meal_lock);
    return counter;
}

static int all_ate(t_program *program)
{
    int i = 0;
    while (i < program->philos[1].num_of_philos)
    {
        if (program->philos[i].num_meals != -1 && finish_meal_counter(program) == program->philos[1].num_of_philos)
        {
            // pthread_mutex_lock(&program->dead_lock);
            // program->dead_flag = 1;
            // pthread_mutex_unlock(&program->dead_lock);
            return 1;
        }
        i++;
    }
    return 0;
    
}

void *monitor(void *arg)
{
    t_program *program = (t_program *)arg;
    //size_t current_time;
    //int i;

    while (1)
    {    
        if (check_dead_flag(&program->philos[0]))
            return arg;

        if (all_ate(program) || check_all_philos(program))
        {
            pthread_mutex_lock(&program->dead_lock);
            program->dead_flag = 1;
            pthread_mutex_unlock(&program->dead_lock);
            return arg;
        }
        
        
        // pthread_mutex_lock(&program->meal_lock); // Lock meal_lock to safely read shared data
        // for (i = 0; i < program->philos->num_of_philos; i++)
        // {
        //     current_time = get_current_time();
        //     // Check if the philosopher has died
        //     if (program->philos[i].eating == 0 && (current_time - program->philos[i].last_meal > program->philos[i].time_to_die))
        //     {
        //         pthread_mutex_unlock(&program->meal_lock); // Unlock before setting the dead flag and printing
        //         pthread_mutex_lock(&program->dead_lock);
        //         *program->philos[i].ptr_dead_flag = 1;
        //         pthread_mutex_unlock(&program->dead_lock);
        //         print_death(&program->philos[i]);
        //         return NULL;
        //     }

        //     // Check if all philosophers have finished eating
        //     if (program->philos[i].num_meals != -1 && program->finished_philo_counter == program->philos->num_of_philos)
        //     {
        //         pthread_mutex_unlock(&program->meal_lock); // Unlock before setting the dead flag
        //         pthread_mutex_lock(&program->dead_lock);
        //         program->dead_flag = 1;
        //         pthread_mutex_unlock(&program->dead_lock);
        //         return NULL;
        //     }
        // }
        // pthread_mutex_unlock(&program->meal_lock); // Unlock after iterating through all philosophers
        
        usleep(250); // Sleep for a short period to reduce CPU usage
    }
    return NULL;
}
