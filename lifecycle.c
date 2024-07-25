/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifecycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 23:27:50 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/25 22:32:56 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

//boolean
int	check_dead_flag(t_philo *philo)
{
	pthread_mutex_lock(&philo->program->dead_lock);
	if (*philo->ptr_dead_flag == 1)
	{
		pthread_mutex_unlock(&philo->program->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->program->dead_lock);
	return (0);
}

//print only if dead flag is 0
static void	ft_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->program->meal_lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->program->meal_lock);
	safe_print(philo, "is sleeping");
	ft_usleep(philo->time_to_sleep);
}

//print only if dead flag is 0
static void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	safe_print(philo, "has taken a fork");
	pthread_mutex_lock(philo->l_fork);
	safe_print(philo, "has taken a fork");
	pthread_mutex_lock(&philo->program->meal_lock);
	philo->last_meal = get_current_time();
	philo->eating = 1;
	pthread_mutex_unlock(&philo->program->meal_lock);
	safe_print(philo, GREEN"is eating"RESET);
	philo->meals_counter++;
	if (philo->num_meals != -1 && philo->meals_counter == philo->num_meals)
	{
		pthread_mutex_lock(&philo->program->meal_lock);
		philo->program->finished_philo_counter++;
		pthread_mutex_unlock(&philo->program->meal_lock);
	}
	ft_usleep(philo->time_to_eat);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

//function executed in each tread
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2 == 0)
		ft_usleep(100);
	if (philo->num_of_philos == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_lock(&philo->program->dead_lock);
		*philo->ptr_dead_flag = 1;
		pthread_mutex_unlock(&philo->program->dead_lock);
		return (arg);
	}
	while (1)
	{
		if (check_dead_flag(philo))
			break ;
		eat(philo);
		ft_sleep(philo);
		safe_print(philo, "is thinking");
	}
	return (arg);
}
