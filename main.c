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

int	main(int argc, char **argv)
{
//data holds data from input
	t_philo		data;
	t_program	set;
    //input check
	if (!input_ok(argc, argv))
		return (EXIT_FAILURE);
    //start program
	//printf("START TIME\n%zu\n", get_current_time() - get_current_time());
	init_input(&data, argv);
    //printf("INPUT: num philo %d\ntime to die %zu\ntime to eat %zu\ntime to sleep %zu\noptional %d\n", data.num_of_philos, data.time_to_die, data.time_to_eat, data.time_to_sleep, data.num_meals);
	if (!init_mutexes(&set, &data))
	{
		printf("Error init mutexes\n");
		return (EXIT_FAILURE);
	}
    //init philos
	set_philosophers(&data, &set);
    //create and join threads and monitor
	if (!create_and_join_threads(&data, &set))
	{
		printf("Error create and join threads\n");
		return (EXIT_FAILURE);
	}
    //destroy and free
	cleanup_all(&set);
	return (0);
}
