/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:06:03 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/23 20:08:02 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

//HEADERS
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>


# define PHILO_MAX 200

//this stricture contains info about each philisopher
typedef struct s_philo
{
	pthread_t		thread;
	int				philo_id;
	int				eating; //1 0
	int				flag_all_ate; //1 0 if meals counter == num_meals
	int				meals_counter; //++
	size_t			last_meal; //start eating + time to eat//shared
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				num_of_philos;
	int				num_meals;
	int				*ptr_dead_flag;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	struct s_program *program;
}   t_philo;

typedef struct s_program
{			
	int				dead_flag; //shared
	int				finished_philo_counter; //shared
	pthread_mutex_t	dead_lock; //shared
	pthread_mutex_t	meal_lock; //shared
	pthread_mutex_t	write_lock; //shared
	pthread_mutex_t *forks; //shared
	t_philo			*philos; //array of structs type t_philo with all info
}					t_program;


//helper functions check input
void	ft_putstr_fd(char *str, int fd);
int		input_ok(int argc, char **argv);
void	init_input(t_philo *philo, char **argv);

//INIT
void	init_input(t_philo *data, char **argv);
void	set_philosophers(t_philo *data, t_program *set);
int	create_and_join_threads(t_philo *data, t_program *set);
int		init_mutexes(t_program *set, t_philo *data);
void	cleanup_all(t_program *set);

//ROUTINE
void	*routine(void *data);
void	*monitor(void *arg);

//UTILS
size_t			get_current_time(void);
unsigned int	ft_atoi(char *str);
void			print_death(t_philo *philo);
int				check_dead_flag(t_philo *philo);
void			safe_print(t_philo *philo, char *str);
int				ft_usleep(size_t milliseconds);

# endif
