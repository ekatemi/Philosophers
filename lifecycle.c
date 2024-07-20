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
        printf("%zu philo id #%d %s\n", get_current_time() - philo->start_time, philo->philo_id, str);
        pthread_mutex_unlock(&philo->program->write_lock);  // Unlock the write mutex after printing
    }
    else
        pthread_mutex_unlock(&philo->program->dead_lock);  // Unlock the mutex if dead flag is set
}

static void print_death(t_philo *philo)
{
    pthread_mutex_lock(&philo->program->dead_lock);  // Lock the mutex for accessing dead_flag
    if (*philo->ptr_dead_flag == 1) 
    {
        pthread_mutex_unlock(&philo->program->dead_lock);  // Unlock the mutex if no dead flag
        pthread_mutex_lock(&philo->program->write_lock);  // Lock the write mutex for printing
        printf("%zu philo id #%d has died\n", get_current_time() - philo->start_time, philo->philo_id);
        pthread_mutex_unlock(&philo->program->write_lock);  // Unlock the write mutex after printing
    }
    else
        pthread_mutex_unlock(&philo->program->dead_lock);  // Unlock the mutex if dead flag is set
}

static void think(t_philo philo)
{
    check_still_alive(&philo);
    safe_print(&philo, " is thinking");  
} 

static void ft_sleep(t_philo philo)
{
    check_still_alive(&philo);
    safe_print(&philo, " is sleeping");
    usleep(philo.time_to_sleep * 1000);///?????
}

//set DEAD FLAG to 0 when last_meal + time_to_die < current time && eating == 0
//or
//all philos have all_ate == 1 and num_meals != -1;

//1 alove, 0 no
void check_still_alive(t_philo *philo)
{
    size_t current_time = get_current_time(); // Current time in milliseconds
    size_t time_since_last_meal = current_time - philo->last_meal; // Time since last meal

    // Debug output to monitor time calculations
    printf("Philo ID %d: Current Time = %ld, Last Meal = %ld, Time Since Last Meal = %ld, Time to DIE = %ld \n",
        philo->philo_id, current_time - philo->start_time, philo->last_meal, time_since_last_meal, philo->time_to_die);

    // Check if the philosopher has been alive longer than `time_to_die`
    if (time_since_last_meal > philo->time_to_die)
    {
        printf("HELLO\n");

        // Check if the philosopher is already marked dead
        
        pthread_mutex_lock(&philo->program->dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(&philo->program->dead_lock); // Unlock before calling print_death

        print_death(philo); // Print death message
    }
}


static void eat(t_philo *philo)
{
    //if philo is not eating and if passsed time_to_die since last meal, set dead flag to 1 
   //set death flag and prints a death message
    if (philo->eating == 0)
        check_still_alive(philo);

    if (philo->philo_id % 2 == 0) {
        usleep(1000);
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

    safe_print(philo, " is eating");
    philo->eating = 1;
    philo->meals_counter++;
    

    printf("EAT FT philo id %d, death_flag %d, meals counter %d, last meal time %ld\n", philo->philo_id, *philo->ptr_dead_flag, philo->meals_counter, philo->last_meal);
    
    //put the forks down
    pthread_mutex_unlock(philo->r_fork);
    pthread_mutex_unlock(philo->l_fork);
    
    pthread_mutex_lock(&philo->program->dead_lock);
    philo->last_meal = get_current_time(); // Update last meal time
    pthread_mutex_unlock(&philo->program->dead_lock);

    pthread_mutex_lock(&philo->program->meal_lock);
    if (philo->num_meals != -1 && philo->meals_counter >= philo->num_meals)
    {
        philo->program->finished_philo_counter++;
        pthread_mutex_unlock(&philo->program->meal_lock);
        
        // pthread_mutex_lock(&philo->program->write_lock);
        // printf("ALL eaten count %d\n", philo->program->finished_philo_counter);
        // pthread_mutex_unlock(&philo->program->write_lock);
        
    }
    else
        pthread_mutex_unlock(&philo->program->meal_lock);
    pthread_mutex_lock(&philo->program->meal_lock);

    if (philo->program->finished_philo_counter == philo->num_of_philos)
    {
        pthread_mutex_unlock(&philo->program->meal_lock);
        pthread_mutex_lock(&philo->program->dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(&philo->program->dead_lock);
    }
    else
        pthread_mutex_unlock(&philo->program->meal_lock);
    
    pthread_mutex_lock(&philo->program->write_lock);
    printf("dead flag in thread =%d\n", *philo->ptr_dead_flag);
    pthread_mutex_unlock(&philo->program->write_lock);
    
    usleep(philo->time_to_eat * 1000);
    philo->eating = 0;
}


//function executed in each tread
void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (philo->num_of_philos == 1)
    {
        usleep(philo->time_to_die * 1000);
        pthread_mutex_lock(&philo->program->dead_lock);
        *philo->ptr_dead_flag = 1;
        pthread_mutex_unlock(&philo->program->dead_lock);
        print_death(philo);
        return arg;
    }

    while (1)
    {
        pthread_mutex_lock(&philo->program->dead_lock);
        if (*philo->ptr_dead_flag == 1)
        {
            pthread_mutex_unlock(&philo->program->dead_lock);
            printf("Thread %d exited because dead_flag is set\n", philo->philo_id);
            break;
        }
        pthread_mutex_unlock(&philo->program->dead_lock);

        pthread_mutex_lock(&philo->program->meal_lock);
        if (philo->program->finished_philo_counter >= philo->num_of_philos)
        {
            pthread_mutex_unlock(&philo->program->meal_lock);
            printf("Thread %d exited because all philosophers finished eating\n", philo->philo_id);
            break;
        }
        pthread_mutex_unlock(&philo->program->meal_lock);

        eat(philo);

        // Add a check after eating
        pthread_mutex_lock(&philo->program->dead_lock);
        if (*philo->ptr_dead_flag == 1)
        {
            pthread_mutex_unlock(&philo->program->dead_lock);
            printf("Thread %d detected dead_flag after eating\n", philo->philo_id);
            break;
        }
        pthread_mutex_unlock(&philo->program->dead_lock);
        ft_sleep(*philo);

        // Add a check after sleeping
        pthread_mutex_lock(&philo->program->dead_lock);
        if (*philo->ptr_dead_flag == 1)
        {
            pthread_mutex_unlock(&philo->program->dead_lock);
            printf("Thread %d detected dead_flag after sleeping\n", philo->philo_id);
            break;
        }
        pthread_mutex_unlock(&philo->program->dead_lock);

        think(*philo);

        // Add a check after thinking
        pthread_mutex_lock(&philo->program->dead_lock);
        if (*philo->ptr_dead_flag == 1)
        {
            pthread_mutex_unlock(&philo->program->dead_lock);
            printf("Thread %d detected dead_flag after thinking\n", philo->philo_id);
            break;
        }
        pthread_mutex_unlock(&philo->program->dead_lock);
    }

    printf("Thread %d fully exited\n", philo->philo_id);
    return arg;
}



