/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 23:01:23 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/25 23:48:10 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_digit(char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

static int	arg_is_valid_num(char *arg)
{
	int	res;

	if (!is_digit(arg))
		return (0);
	res = ft_atoi(arg);
	return (res > 0);
}

int	input_ok(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("Wrong argument count", 2);
		return (0);
	}
	while (i < argc)
	{
		if (!arg_is_valid_num(argv[i]))
		{
			ft_putstr_fd("Only numbers > 0\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}
