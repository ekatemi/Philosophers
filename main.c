/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:48:20 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/23 20:05:13 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
ARGUMENTS
number_of_philosophers
time_to_die time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]
*/

void	safe_print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->program->dead_lock);
	if (*philo->ptr_dead_flag == 0)
	{
		pthread_mutex_unlock(&philo->program->dead_lock);
		pthread_mutex_lock(&philo->program->write_lock);
		printf("%zu %d %s\n", get_current_time() - philo->start_time,
			philo->philo_id, str);
		pthread_mutex_unlock(&philo->program->write_lock);
	}
	else
		pthread_mutex_unlock(&philo->program->dead_lock);
}

// //only prints death, so dead flag should be set to 1 before or after
// void	print_death(t_philo *philo, char *str)
// {
// 	pthread_mutex_lock(&philo->program->write_lock);
// 	printf("%zu %d %s\n", get_current_time()
// 		- philo->start_time, philo->philo_id, str);
// 	pthread_mutex_unlock(&philo->program->write_lock);
// }

int	main(int argc, char **argv)
{
	t_philo		data;
	t_program	set;

	if (!input_ok(argc, argv))
		return (EXIT_FAILURE);
	init_input(&data, argv);
	if (!init_mutexes(&set, &data))
	{
		printf("Error init mutexes\n");
		return (EXIT_FAILURE);
	}
	set_philosophers(&data, &set);
	if (!create_and_join_threads(&data, &set))
	{
		printf("Error create and join threads\n");
		return (EXIT_FAILURE);
	}
	cleanup_all(&set);
	return (0);
}
