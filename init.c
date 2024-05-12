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
	int	 i;
	i = 0;
	start_time = get_current_time();
	while(i < data->num_of_philos)
	{
		set->philos[i].philo_id = i + 1;
		set->philos[i].eating = 0;
		set->philos[i].meals_eaten = 0;
		set->philos[i].all_ate = 0;
		set->philos[i].last_meal = start_time;
		set->philos[i].start_time = start_time; //same for everyone
		set->philos[i].num_of_philos = data->num_of_philos; //same for everyone
		set->philos[i].time_to_die = data->time_to_die; //same for everyone
		set->philos[i].time_to_eat = data->time_to_eat; //same for everyone
		set->philos[i].time_to_sleep = data->time_to_sleep; //same for everyone
		set->philos[i].num_meals = data->num_meals; //same for everyone
		i++;
	}
}

void init_mutexes(t_program *set)
{
	int	i;

	i = 0;
	set->forks = malloc(set->philos->num_of_philos * sizeof(pthread_mutex_t));
	set->philos = malloc(set->philos->num_of_philos * sizeof(t_philo));
	set->dead_flag = 0;
	while (i < set->philos->num_of_philos)
	{
		pthread_mutex_init(&set->forks[i], NULL);
		set->philos[i].l_fork = &set->forks[i];
		set->philos[i].r_fork = &set->forks[(i + 1) % set->philos->num_of_philos];
		set->philos[i].dead = &set->dead_flag;
		set->philos[i].write_lock = &set->write_lock;
		set->philos[i].meal_lock = &set->meal_lock;
		set->philos[i].dead_lock = &set->dead_lock;
		pthread_mutex_init(set->philos[i].meal_lock, NULL);
		pthread_mutex_init(set->philos[i].dead_lock, NULL);
		i++;
	}
}

void cleanup_all(t_program *set)
{
	
	int	i;

	i = 0;
	while (i < set->philos->num_of_philos)
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

void	create_and_join_threads(t_philo *data, t_program *set)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_create(&set->philos[i].thread, NULL, &routine, &set->philos[i]))
		{
			ft_putstr_fd("Error thread create", 2);
			cleanup_all(set);
			exit(EXIT_FAILURE);// not shure
		}
		i++;
	}
	
	i = 0;
	while (i < data->num_of_philos)
	{
		if(pthread_join(set->philos[i].thread, NULL))
		{
			ft_putstr_fd("Error thread join", 2);
			cleanup_all(set);
			exit(EXIT_FAILURE);// not shure	
		}
		i++;
	}
}