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

void	init_input(t_philo *data, char **argv)
{
	data->num_of_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->num_meals = ft_atoi(argv[5]);
	else
		data->num_meals = -1;
}

void	set_philosophers(t_philo *data, t_program *set)
{
	size_t	start_time;
	int		i;

	i = 0;
	start_time = get_current_time();
	set->dead_flag = 0;
	set->finished_philo_counter = 0;
	while (i < data->num_of_philos)
	{
		set->philos[i].philo_id = i + 1;
		set->philos[i].eating = 0;
		set->philos[i].meals_counter = 0;
		set->philos[i].flag_all_ate = 0;
		set->philos[i].last_meal = start_time;
		set->philos[i].start_time = start_time;
		set->philos[i].num_of_philos = data->num_of_philos;
		set->philos[i].time_to_die = data->time_to_die;
		set->philos[i].time_to_eat = data->time_to_eat;
		set->philos[i].time_to_sleep = data->time_to_sleep;
		set->philos[i].num_meals = data->num_meals;
		set->philos[i].ptr_dead_flag = &set->dead_flag;
		set->philos[i].program = set;
		i++;
	}
}

int	init_mutexes(t_program *set, t_philo *data)
{
	int	i;

	i = 0;
	set->philos = malloc(data->num_of_philos * sizeof(t_philo));
	if (!set->philos)
	{
		ft_putstr_fd("Malloc philos err", 2);
		return (0);
	}
	set->forks = malloc(data->num_of_philos * sizeof(pthread_mutex_t));
	if (!set->forks)
	{
		ft_putstr_fd("Malloc forks err", 2);
		return (0);
	}
	pthread_mutex_init(&set->write_lock, NULL);
	pthread_mutex_init(&set->meal_lock, NULL);
	pthread_mutex_init(&set->dead_lock, NULL);
	while (i < data->num_of_philos)
	{
		pthread_mutex_init(&set->forks[i], NULL);
		set->philos[i].l_fork = &set->forks[i];
		set->philos[i].r_fork = &set->forks[(i + 1) % data->num_of_philos];
		set->philos[i].ptr_dead_flag = &set->dead_flag;
		i++;
	}
	return (1);
}

void	cleanup_all(t_program *set)
{
	int	i;

	i = 0;
	while (i < set->philos[0].num_of_philos)
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

int	create_and_join_threads(t_philo *data, t_program *set)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	if (pthread_create(&monitor_thread, NULL, monitor, set))
	{
		ft_putstr_fd("Error monitor thread create", 2);
		cleanup_all(set);
		return (0);
	}
	while (i < data->num_of_philos)
	{
		if (pthread_create(&set->philos[i].thread,
				NULL, routine, &set->philos[i]))
		{
			ft_putstr_fd("Error thread create", 2);
			cleanup_all(set);
			return (0);
		}
		i++;
	}
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_join(set->philos[i].thread, NULL))
		{
			ft_putstr_fd("Error thread join", 2);
			cleanup_all(set);
			return (0);
		}
		i++;
	}
	if (pthread_join(monitor_thread, NULL))
	{
		ft_putstr_fd("Error monitor thread join", 2);
		cleanup_all(set);
		return (0);
	}
	return (1);
}
