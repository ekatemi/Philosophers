#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died


static void safe_print(t_program *set, char *str)
{
    pthread_mutex_lock(&set->dead_lock);  // Lock the mutex for accessing dead_flag
    if (set->philos->ptr_dead_flag == 0) 
    {
        pthread_mutex_unlock(&set->dead_lock);  // Unlock the mutex if no dead flag
        pthread_mutex_lock(&set->write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d %s\n", get_current_time(), set->philos->philo_id, str);
        pthread_mutex_unlock(&set->write_lock);  // Unlock the write mutex after printing
    }
    else
        pthread_mutex_unlock(&set->dead_lock);  // Unlock the mutex if dead flag is set
}

static void print_death(t_program *set)
{
    pthread_mutex_lock(&set->dead_lock);  // Lock the mutex for accessing dead_flag
    if (set->dead_flag == 1) 
    {
        pthread_mutex_unlock(&set->dead_lock);  // Unlock the mutex if no dead flag
        pthread_mutex_lock(&set->write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d has died\n", get_current_time(), set->philos->philo_id);
        pthread_mutex_unlock(&set->write_lock);  // Unlock the write mutex after printing
    }
    pthread_mutex_unlock(&set->dead_lock);  // Unlock the mutex if dead flag is set
}

static void think(t_program *set)
{
    safe_print(set, " is thinking");  
} 

static void ft_sleep(t_program *set)
{
    safe_print(set, " is sleeping");
    usleep(set->philos->time_to_sleep * 1000);///?????
}

//set DEAD FLAG to 0 when last_meal + time_to_die < current time && eating == 0
//or
//all philos have all_ate == 1 and num_meals != -1;
static void eat(t_program *set)
{
    //if philo is not eating and if passsed time_to_die since last meal, set dead flag to 1 
    if(!set->philos->eating && (set->philos->last_meal + set->philos->time_to_die) < get_current_time())
    {
        pthread_mutex_lock(&set->dead_lock);
        set->dead_flag = 1;
        pthread_mutex_unlock(&set->dead_lock);
        print_death(set);
    }

    if (set->philos->philo_id % 2 == 0)
	    usleep(1000);
    // Lock the right fork
    pthread_mutex_lock(set->philos->r_fork);//
    safe_print(set, " has taken right fork");
    //Lock the left fork
    pthread_mutex_lock(set->philos->l_fork);//
    safe_print(set, " has taken left fork");
    set->philos->last_meal = get_current_time();
    safe_print(set, " is eating");
    set->philos->meals_counter++;
    set->philos->eating = 1;
    //put the forks down
    pthread_mutex_unlock(set->philos->l_fork);
    pthread_mutex_unlock(set->philos->r_fork);
    
    if (set->philos->num_meals != -1 && set->philos->meals_counter == set->philos->num_meals)
    {
        pthread_mutex_lock(&set->meal_lock);
        set->finished_philo_counter++;
        pthread_mutex_unlock(&set->meal_lock);
    }
    usleep(set->philos->time_to_eat * 1000);
}

// static void set_all_ate(t_philo *philo)
// {
//     //int res = 0; // Assume all philosophers have eaten until proven otherwise
//     int i = 0;
//     while (i < philo[0].num_of_philos)
//     {
//         if (philo[i].meals_counter == philo[i].num_meals)
//         {
//             pthread_mutex_lock(philo[i].ptr_meal_lock);
//             *philo[i].all_philo_ate++;
//             pthread_mutex_unlock(philo[i].ptr_meal_lock);
//         }
//         i++;
//     }
// }


//function executed in each tread
void *routine(void *arg)
{
    t_philo *phoilo;

    set = (t_program *)arg;
   
    if (set->philos->num_of_philos == 1)
    {
        usleep(set->philos->time_to_die * 1000);
        pthread_mutex_lock(&set->dead_lock);
        set->dead_flag = 1;
        pthread_mutex_unlock(&set->dead_lock);
        print_death(set);
        return arg;
    }
    
    //printf("General meals count %d\n", set->finished_philo_counter);

    while (set->dead_flag != 1)
    {
        pthread_mutex_lock(&set->meal_lock);
        if (set->finished_philo_counter == set->philos->num_meals)
        {
            pthread_mutex_unlock(&set->meal_lock);
            pthread_mutex_lock(&set->dead_lock);
            set->dead_flag = 1;
            pthread_mutex_unlock(&set->dead_lock);
            break ;
        }
        pthread_mutex_unlock(&set->meal_lock);
            
        eat(set);

        ft_sleep(set);
        think(set);
    }
    //print_death(philo);
   // printf("Hello from philo %d, my left fork is %p and right forkis %p and meal counter is %d\n", set->philos->philo_id, set->philos->l_fork, set->philos->r_fork, set->philos->meals_counter);
   // printf("Dead flag %d\n", set->dead_flag);

    return arg;
}

