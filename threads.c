#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died


// Check if any philo died

int	check_if_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].num_of_philos)
	{
		pthread_mutex_lock(&philos[0].program->meal_lock);
        if (get_current_time() - philos[i].last_meal >= philos[0].time_to_die)
		{
			pthread_mutex_unlock(&philos[0].program->meal_lock);
            print_death(&philos[i]);
			// pthread_mutex_lock(&philos[0].program->dead_lock);
			// *philos->ptr_dead_flag = 1;
			// pthread_mutex_unlock(&philos[0].program->dead_lock);
			return (1);
		}
        pthread_mutex_unlock(&philos[0].program->meal_lock);
		i++;
	}
	return (0);
}

// Checks if all the philos ate the num_of_meals

int	check_if_all_ate(t_philo *philos)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].num_meals == -1)
		return (0);
	while (i < philos[0].num_of_philos)
	{
		pthread_mutex_lock(&philos[i].program->meal_lock);
		if (philos[i].meals_counter >= philos[i].num_meals)
			finished_eating++;
		pthread_mutex_unlock(&philos[i].program->meal_lock);
		i++;
	}
	if (finished_eating == philos[0].num_of_philos)
	{

		return (1);
	}
	return (0);
}

// Monitor thread routine

void	*monitor(void *pointer)
{
	t_philo	*philos;

	philos = (t_philo *)pointer;
	while (1)
		if (check_if_dead(philos) == 1 || check_if_all_ate(philos) == 1)
        {
            pthread_mutex_lock(&philos[0].program->dead_lock);
		    *philos->ptr_dead_flag = 1;
		    pthread_mutex_unlock(&philos[0].program->dead_lock);
            break ;
        }
			
	return (pointer);
}
//MY VERS
// void *monitor(void *arg)
// {
//     t_program *program = (t_program *)arg;
//     size_t current_time;
//     int i;

//     while (1)
//     {
//         pthread_mutex_lock(&program->meal_lock); // Lock meal_lock to safely read shared data
//         i = 0;
//         while (i < program->philos->num_of_philos)
//         {
//             current_time = get_current_time();
//             // Check if the philosopher has died
//             if (current_time - program->philos[i].last_meal > program->philos[i].time_to_die)
//             {
//                 pthread_mutex_unlock(&program->meal_lock); // Unlock before setting the dead flag and printing
//                 pthread_mutex_lock(&program->dead_lock);
//                 *program->philos[i].ptr_dead_flag = 1;
//                 pthread_mutex_unlock(&program->dead_lock);
//                 print_death(&program->philos[i]);
//                 return NULL;
//             }

//             // Check if all philosophers have finished eating
//             if (program->philos[i].num_meals != -1 && program->finished_philo_counter == program->philos->num_of_philos)
//             {
//                 pthread_mutex_unlock(&program->meal_lock); // Unlock before setting the dead flag
//                 pthread_mutex_lock(&program->dead_lock);
//                 program->dead_flag = 1;
//                 pthread_mutex_unlock(&program->dead_lock);
//                 return NULL;
//             }

//             i++; // Increment the index
//         }
//         pthread_mutex_unlock(&program->meal_lock); // Unlock after iterating through all philosophers
        
//         //usleep(250); // Sleep for a short period to reduce CPU usage
//     }
//     return NULL;
// }

