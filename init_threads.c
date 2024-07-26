/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 20:56:46 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/26 22:27:59 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_dead_flag(t_program *set)
{
	pthread_mutex_lock(&set->dead_lock);
	set->dead_flag = 1;
	pthread_mutex_unlock(&set->dead_lock);
}

int	cleanup_and_return(t_program *set)
{
	cleanup_all(set);
	return (0);
}

int	create_and_join_philos(t_philo *data, t_program *set)
{
	int			i;

	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_create(&set->philos[i].thread,
				NULL, routine, &set->philos[i]))
		{
			ft_putstr_fd("Error thread create\n", 2);
			set_dead_flag(set);
			break ;
		}
		i++;
	}
	i--;
	while (i >= 0)
	{
		if (pthread_join(set->philos[i].thread, NULL))
		{
			ft_putstr_fd("Error thread join", 2);
			cleanup_and_return(set);
		}
		i--;
	}
	return (1);
}

int	create_and_join_threads(t_philo *data, t_program *set)
{
	pthread_t	monitor_thread;

	if (pthread_create(&monitor_thread, NULL, monitor, set))
	{
		ft_putstr_fd("Error monitor thread create", 2);
		cleanup_and_return(set);
	}
	if (!create_and_join_philos(data, set))
		return (0);
	if (pthread_join(monitor_thread, NULL))
	{
		ft_putstr_fd("Error monitor thread join", 2);
		cleanup_and_return(set);
	}
	return (1);
}
