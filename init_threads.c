/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 20:56:46 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/25 20:56:53 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_program *set)
{
	int	i;

	i = 0;
	while (i < set->num_of_philos)
	{
		if (pthread_create(&set->philos[i].thread, NULL, routine, &set->philos[i]))
        {
            ft_putstr_fd("Error thread create", 2);
            printf("Created %d threads from %d\n", i, set->num_of_philos);
            pthread_mutex_lock(&set->dead_lock);
            set->dead_flag = 1; // Set the dead flag
            pthread_mutex_unlock(&set->dead_lock);
            set->num_of_philos = i; // Update the number of created threads
            return (0);
        }
		i++;
	}
	return (1);
}

int	join_threads(t_program *set)
{
	int	i;

	i = 0;
	while (i < set->num_of_philos)
	{
		if (pthread_join(set->philos[i].thread, NULL))
		{
			ft_putstr_fd("Error thread join", 2);
			cleanup_all(set);
			return (0);
		}
		i++;
	}
	return (1);
}

int	create_and_join_threads(t_program *set)
{
	if (!create_threads(set))
		return (0);
	if (!join_threads(set))
		return (0);
	return (1);
}
