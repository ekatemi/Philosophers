#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

//ROUTINES
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

//chack if all fh
    // pthread_mutex_lock(&philo->program->meal_lock);
    // if (philo->program->finished_philo_counter == philo->num_of_philos)
    // {
    //     pthread_mutex_unlock(&philo->program->meal_lock);
    //     pthread_mutex_lock(&philo->program->dead_lock);
    //     *philo->ptr_dead_flag = 1;
    //     pthread_mutex_unlock(&philo->program->dead_lock);
    // }
    // pthread_mutex_unlock(&philo->program->meal_lock);
//if one philo have died it sets dead for all
//waits till all_ate (this also sets dead flag)

//iterates all threads checking time to starve, if so sets dead flag and prints time of death
//1 dead 0 not

//here err
// Checks if the philosopher is dead

// int philosopher_dead(t_philo *philo, size_t time_to_die) {
//     if (philo == NULL || philo->program == NULL) return 0;

//     pthread_mutex_lock(&philo->program->meal_lock);
//     int is_dead = (get_current_time() - philo->last_meal >= time_to_die && philo->eating == 0);
//     pthread_mutex_unlock(&philo->program->meal_lock);

//     return is_dead;
// }

// // Check if any philo died

// int check_if_dead(t_philo *philos) {
//     if (philos == NULL || philos[0].program == NULL) return 0;

//     for (int i = 0; i < philos[0].num_of_philos; i++) {
//         if (philosopher_dead(&philos[i], philos[0].time_to_die)) {
//             pthread_mutex_lock(&philos[0].program->dead_lock);
//             if (*philos[i].ptr_dead_flag == 0) {
//                 *philos[i].ptr_dead_flag = 1;
//                 pthread_mutex_unlock(&philos[0].program->dead_lock);
//                 print_death(&philos[i]);
//                 return 1;
//             }
//             pthread_mutex_unlock(&philos[0].program->dead_lock);
//         }
//     }
//     return 0;
// }

// static int check_for_all_eaten(t_program *set)
// {
//     pthread_mutex_lock(&set->meal_lock);
//     {
//         if (set->finished_philo_counter)
//         pthread_mutex_unlock(&set->meal_lock);
        
//         // pthread_mutex_lock(&set->write_lock);
//         // printf("All filos finished eating\n");
//         // pthread_mutex_unlock(&set->write_lock);

//         pthread_mutex_lock(&set->dead_lock);
//         set->dead_flag = 1;
//         pthread_mutex_unlock(&set->dead_lock);
//         return 1;
//     }
//     pthread_mutex_unlock(&set->meal_lock);
//     return 0;
// }

// static int check_is_eating(t_philo *philo)
// {
//     pthread_mutex_lock(&philo->program->meal_lock);
//     if (philo->eating == 1)
//     {
//         pthread_mutex_unlock(&philo->program->meal_lock);
//         return 1;
//     }
//     pthread_mutex_unlock(&philo->program->meal_lock);
//     return 0;
// }

void *monitor(void *arg)
{
    t_program *program = (t_program *)arg;
    size_t current_time;
    int i;

    while (1)
    {
        pthread_mutex_lock(&program->meal_lock); // Lock meal_lock to safely read shared data
        for (i = 0; i < program->philos->num_of_philos; i++)
        {
            current_time = get_current_time();
            // Check if the philosopher has died
            if (program->philos[i].eating == 0 && (current_time - program->philos[i].last_meal > program->philos[i].time_to_die))
            {
                pthread_mutex_unlock(&program->meal_lock); // Unlock before setting the dead flag and printing
                pthread_mutex_lock(&program->dead_lock);
                *program->philos[i].ptr_dead_flag = 1;
                pthread_mutex_unlock(&program->dead_lock);
                print_death(&program->philos[i]);
                return NULL;
            }

            // Check if all philosophers have finished eating
            if (program->philos[i].num_meals != -1 && program->finished_philo_counter == program->philos->num_of_philos)
            {
                pthread_mutex_unlock(&program->meal_lock); // Unlock before setting the dead flag
                pthread_mutex_lock(&program->dead_lock);
                program->dead_flag = 1;
                pthread_mutex_unlock(&program->dead_lock);
                return NULL;
            }
        }
        pthread_mutex_unlock(&program->meal_lock); // Unlock after iterating through all philosophers
        
        usleep(500); // Sleep for a short period to reduce CPU usage
    }
    return NULL;
}
