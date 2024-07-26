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
		printf("%zu %d %s\n", get_current_time() - philo->program->start_time,
			philo->philo_id, str);
		pthread_mutex_unlock(&philo->program->write_lock);
	}
	else
		pthread_mutex_unlock(&philo->program->dead_lock);
}

int	main(int argc, char **argv)
{
	//t_philo		data;
	t_program	set;
	pthread_t	monitor_thread;

	if (!input_ok(argc, argv))
		return (EXIT_FAILURE);
	init_input(&set, argv);
	if (!set_philosophers(&set))
		return (EXIT_FAILURE);
	if (!init_mutexes(&set))
	{
		printf("Error init mutexes\n");
		return (EXIT_FAILURE);
	}
	if (pthread_create(&monitor_thread, NULL, monitor, &set))
	{
		ft_putstr_fd("Error monitor thread create", 2);
		cleanup_all(&set);
		return (EXIT_FAILURE);
	}
	// Detach the monitor thread
    if (pthread_detach(monitor_thread))
    {
        ft_putstr_fd("Error monitor thread detach", 2);
        cleanup_all(&set);
        return (EXIT_FAILURE);
    }
	if (!create_and_join_threads(&set))
	{
		ft_putstr_fd("Error philo thread create", 2);
		cleanup_all(&set);
		return (EXIT_FAILURE);
	}
	cleanup_all(&set);
	return (EXIT_SUCCESS);
}
