/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 23:01:23 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/22 23:17:23 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_input_err(int code)
{
	if (code == 1)
		ft_putstr_fd("Only numbers allowed", 2);
	if (code == 2)
		ft_putstr_fd("Only numbers > 0", 2);
	return (0);
}

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

static int	arg_is_digit(char *arg)
{
	while (*arg)
	{
		if (!is_digit(arg))
			return (0);
		arg++;
	}
	return (1);
}

int	input_ok(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc >= 5 && argc <= 6)
	{
		while (i < argc)
		{
			if (!arg_is_digit(argv[i]))
				ft_input_err(1);
			else if (ft_atoi(argv[i]) == 0)
				ft_input_err(2);
			i++;
		}
	}
	else
	{
		ft_putstr_fd("Wrong argument count", 2);
		return (0);
	}
	return (1);
}
