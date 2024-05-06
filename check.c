#include "philo.h"

static int is_digit(char *str)
{
    while(*str)
    {
        if (*str < '0' || *str > '9')
            return (0);
        str++;
    }
    return (1);
}

static int arg_is_digit(char *arg)
{
    while (*arg)
    {
        if (!is_digit(arg))
            return (0);
        arg++;
    }
    return (1);
}

int input_ok(int argc, char **argv)
{
    int i = 1;
    if (argc >= 5 && argc <= 6)
    {
        while (i < argc)
        {
            if (!arg_is_digit(argv[i]))
            {
                ft_putstr_fd("Only numbers allowed", 2);
                return (0);
            }
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