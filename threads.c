#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

//ROUTINES

// void *eat_routine(t_philo data)
// {
//     //mutex/
//     usleep(data.time_to_eat);
//     //data.time_to_die = 
//     //end mutex
// }

// void *sleep_routine(t_philo data)
// {
//     //mutex/
//     usleep(data.time_to_sleep);
//     //end mutex
// }



//we set dead flag to 1 if current time > last_meal + time_to_die
static void set_dead_flag(t_program *set)
{
   int i = 0;
   size_t curr_time = get_current_time();
   //printf("Dead flag %d\n", *(set->philos[0].ptr_dead_flag));
   while(i < set->philos[0].num_of_philos)
   {
        pthread_mutex_lock(set->philos[i].ptr_dead_lock);
            if (curr_time > set->philos[i].last_meal + set->philos[i].time_to_die && !set->philos[i].eating)
            {
                set->dead_flag = 1;
                pthread_mutex_unlock(set->philos[i].ptr_dead_lock);
                break ;
            }        
        pthread_mutex_unlock(set->philos[i].ptr_dead_lock);
    i++;
   }
   //printf("Dead flag %d\n", set->dead_flag);
}

static void set_all_ate(t_philo *philo)
{
    if (philo->num_meals == philo->meals_eaten)//this I increment in eat routine
       philo->all_ate = 1;
}
//if all_philosophers has eaten n meals, return 1
static int all_meals_eaten(t_program *set)
{
    int i = 0;
    int finished_eating = 0;
    if (set->philos[0].num_meals == -1)
        return 0;
    while (i < set->philos[0].num_of_philos)//till i < num threads
    {
        set_all_ate(&set->philos[i]);
        
        pthread_mutex_lock(set->philos[i].ptr_meal_lock);
        if (set->philos[i].all_ate == 1)
            finished_eating++;
        pthread_mutex_unlock(set->philos[i].ptr_meal_lock);  
        i++;
    }
    if (finished_eating == set->philos[0].num_of_philos)
    {
        pthread_mutex_lock(&set->dead_lock);
 		set->dead_flag = 1;//need it to finish all threads
 		pthread_mutex_unlock(&set->dead_lock);
        return 1;
    }
    return 0;
}

//if one philo have died it sets dead for all
//waits till all_ate
void *monitor(void *arg)
{
    t_program *set;
    set = (t_program *)arg;

    while(1)
    {
        printf("Hey from monitor loop\n");
        pthread_mutex_lock(&set->dead_lock);
        set_dead_flag(set);
        pthread_mutex_unlock(&set->dead_lock);
        pthread_mutex_lock(&set->meal_lock);
        all_meals_eaten(set);
        pthread_mutex_unlock(&set->meal_lock);
        if (set->dead_flag == 1)
            break ;
        usleep(500);//gpt suggestion...maybe sleep time_to_eat?
    }
    return (arg);
}