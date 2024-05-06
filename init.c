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

void	start_philosophers(t_philo *data, t_program *set)
{
	size_t	start_time;
	int	 i;
	i = 0;
	start_time = get_current_time();
	while(i < data->num_of_philos)
	{
		set->philos[i].philo_id = i + 1;
		set->philos[i].last_meal = start_time;
		set->philos[i].start_time = start_time;
		set->philos[i].num_of_philos = data->num_of_philos;
		set->philos[i].time_to_die = data->time_to_die;
		set->philos[i].time_to_eat = data->time_to_eat;
		set->philos[i].time_to_sleep = data->time_to_sleep;
		set->philos[i].num_meals = data->num_meals;
		set->philos[i].all_ate = 0;
		if (pthread_create(&set->philos[i].thread, NULL, &routine, &set->philos[i]))
		{
			ft_putstr_fd("Error thread create", 2);
			exit(EXIT_FAILURE);// not shure
		}
		i++;
	}
}

void	join_philosophers(t_philo *data, t_program *set)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_join(set->philos[i].thread, NULL);
		i++;
	}
}