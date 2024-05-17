#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died


static void safe_print(t_philo *philo, char *str)
{
    pthread_mutex_lock(&philo->program->dead_lock);  // Lock the mutex for accessing dead_flag
    if (*philo->ptr_dead_flag == 0) 
    {
        pthread_mutex_unlock(&philo->program->dead_lock);  // Unlock the mutex if no dead flag
        pthread_mutex_lock(&philo->program->write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d %s\n", get_current_time(), philo->philo_id, str);
        pthread_mutex_unlock(&philo->program->write_lock);  // Unlock the write mutex after printing
    }
    else
        pthread_mutex_unlock(&philo->program->write_lock);  // Unlock the mutex if dead flag is set
}

static void print_death(t_philo *philo)
{
    pthread_mutex_lock(&philo->program->dead_lock);  // Lock the mutex for accessing dead_flag
    if (*philo->ptr_dead_flag == 1) 
    {
        pthread_mutex_unlock(&philo->program->dead_lock);  // Unlock the mutex if no dead flag
        pthread_mutex_lock(&philo->program->write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d has died\n", get_current_time(), philo->philo_id);
        pthread_mutex_unlock(&philo->program->write_lock);  // Unlock the write mutex after printing
    }
    pthread_mutex_unlock(&philo->program->dead_lock);  // Unlock the mutex if dead flag is set
}

static void think(t_philo philo)
{
    safe_print(&philo, " is thinking");  
} 

static void ft_sleep(t_philo philo)
{
    safe_print(&philo, " is sleeping");
    usleep(philo.time_to_sleep * 1000);///?????
}

//set DEAD FLAG to 0 when last_meal + time_to_die < current time && eating == 0
//or
//all philos have all_ate == 1 and num_meals != -1;

//1 alove, 0 no
void check_still_alive(t_philo *philo)
{
    size_t time = get_current_time();
    if(philo->eating == 0 && (philo->last_meal + philo->time_to_die) < time)
    {
        pthread_mutex_lock(&philo->program->dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(&philo->program->dead_lock);
        print_death(philo);
    }
}


static void eat(t_philo *philo)
{
    //if philo is not eating and if passsed time_to_die since last meal, set dead flag to 1 
   //set death flag and prints a death message
    check_still_alive(philo);

    if (philo->philo_id % 2 == 0)
	    usleep(1000);
    
    // Lock the right fork
    pthread_mutex_lock(philo->r_fork);//
    safe_print(philo, " has taken right fork");
    //Lock the left fork
    pthread_mutex_lock(philo->l_fork);//
    safe_print(philo, " has taken left fork");
    
    philo->last_meal = get_current_time();
    safe_print(philo, " is eating");

    philo->eating = 1;
    philo->meals_counter++;
    
    printf("philo id %d, death_flag %d, meals counter %d\n", philo->philo_id, *philo->ptr_dead_flag, philo->meals_counter);
    
    //put the forks down
    pthread_mutex_unlock(philo->l_fork);
    pthread_mutex_unlock(philo->r_fork);
    

    pthread_mutex_lock(&philo->program->meal_lock);
    if (philo->num_meals != -1 && philo->meals_counter >= philo->num_meals)
    {
        philo->program->finished_philo_counter++;
        pthread_mutex_unlock(&philo->program->meal_lock);
        
        pthread_mutex_lock(&philo->program->write_lock);
        printf("ALL eaten count %d\n", philo->program->finished_philo_counter);
        pthread_mutex_unlock(&philo->program->write_lock);
        
    }
    else
        pthread_mutex_unlock(&philo->program->meal_lock);
    pthread_mutex_lock(&philo->program->meal_lock);
    if (philo->program->finished_philo_counter >= philo->num_of_philos)
    {
        pthread_mutex_unlock(&philo->program->meal_lock);
        pthread_mutex_lock(&philo->program->dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(&philo->program->dead_lock);
    }
    pthread_mutex_unlock(&philo->program->meal_lock);
    
    pthread_mutex_lock(&philo->program->write_lock);
    printf("dead flag %d\n", *philo->ptr_dead_flag);
    pthread_mutex_unlock(&philo->program->write_lock);
    
    usleep(philo->time_to_eat * 1000);
}


//function executed in each tread
void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
   
    if (philo->num_of_philos == 1)
    {
        usleep(philo->time_to_die * 1000);
        pthread_mutex_lock(&philo->program->dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(&philo->program->dead_lock);
        print_death(philo);
        return arg;
    }
    
    //printf("General meals count %d\n", set->finished_philo_counter);
    //pthread_mutex_lock(&philo->program->dead_lock);
    while (1) 
    {
        pthread_mutex_lock(&philo->program->dead_lock);
        if (*philo->ptr_dead_flag == 1)
        {
            pthread_mutex_unlock(&philo->program->dead_lock);
            printf("thread #exited because dead_flag\n");
            break ;
        }
        else
            pthread_mutex_unlock(&philo->program->dead_lock);
        
        eat(philo);
        ft_sleep(*philo);
        think(*philo);      
    }
    printf("Hello from philo %d, my left fork is %p and right forkis %p and meal counter is %d\n", philo->philo_id, philo->l_fork, philo->r_fork, philo->meals_counter);
    printf("Dead flag %d\n", philo->program->dead_flag);

    return arg;
}

