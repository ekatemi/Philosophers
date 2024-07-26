/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 23:19:08 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/22 23:26:38 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_input(t_program *set, char **argv)
{
	set->num_of_philos = ft_atoi(argv[1]);
	set->time_to_die = ft_atoi(argv[2]);
	set->time_to_eat = ft_atoi(argv[3]);
	set->time_to_sleep = ft_atoi(argv[4]);
	set->start_time = get_current_time();
	set->finished_philo_counter = 0;
	set->dead_flag = 0;// 1
	if (argv[5])
		set->num_meals = ft_atoi(argv[5]);
	else
		set->num_meals = -1;
}

int	set_philosophers(t_program *set)
{
	int		i;

	i = 0;
	set->philos = malloc(set->num_of_philos * sizeof(t_philo));
	if (!set->philos)
		return (0);
	while (i < set->num_of_philos)
	{
		set->philos[i].philo_id = i + 1;
		set->philos[i].eating = 0;
		set->philos[i].meals_counter = 0;
		set->philos[i].last_meal = set->start_time;
		set->philos[i].ptr_dead_flag = &set->dead_flag;
		set->philos[i].program = set;
		i++;
	}
	return (1);
}

int	init_mutexes(t_program *set)
{
	int	i;

	i = 0;
	set->forks = malloc(set->num_of_philos * sizeof(pthread_mutex_t));
	if (!set->forks)
		return (0);
	if (pthread_mutex_init(&set->write_lock, NULL) != 0 ||
	pthread_mutex_init(&set->meal_lock, NULL) != 0 ||
	pthread_mutex_init(&set->dead_lock, NULL) != 0)
	{
		return (0);
	}
	while (i < set->num_of_philos)
	{
		pthread_mutex_init(&set->forks[i], NULL);
		set->philos[i].l_fork = &set->forks[i];
		set->philos[i].r_fork = &set->forks[(i + 1) % set->num_of_philos];
		i++;
	}
	return (1);
}

void	cleanup_all(t_program *set)
{
	int	i;

	i = 0;
	while (i < set->num_of_philos)
	{
		pthread_mutex_destroy(&set->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&set->meal_lock);
	pthread_mutex_destroy(&set->dead_lock);
	pthread_mutex_destroy(&set->write_lock);
	free(set->forks);
	free(set->philos);
}
