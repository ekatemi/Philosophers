#include "philo.h"

int is_digit(char *str)
{
    while(*str)
    {
        if (*str < '0' || *str > '9')
            return (0);
        str++;
    }
    return (1);
}

static int ft_atoi(char *str)
{
    int sign;
    int res;

    sign = 1;
    res = 0;
    while (*str == ' ' || *str == '\n' || *str == '\t')
        str++;
    if (*str == '-')
        sign = -1;
    if (*str == '-' || *str == '+')
        str++;
    while (*str >= '0' && *str <= '9')
    {
        res = res * 10 + (*str - '0');
        str++;
    }
    return (res * sign);
}


void ft_putstr_fd(char *str, int fd)
{
    if (str == NULL || fd < 0)
        return;  
    while (*str)
        write(fd, str++, 1);
}

static void struct_init(t_philo *data)
{
    data->num_philo = 0;
    data->num_forks = 0;
    data->time_to_die = 0;
    data->time_to_eat = 0;
    data->time_to_sleep = 0;
    data->num_time_to_eat = NUM_MEALS;
}

int to_microseconds(int milli)
{
    return (milli * 1000);
}

void set_start_time(t_philo *data)
{
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
}

void handle_input(t_philo *data, char **argv)
{
    struct_init(data); //maybe no need
    data->num_philo = ft_atoi(argv[1]);
    data->num_forks = data->num_philo;
    data->time_to_die = to_microseconds(ft_atoi(argv[2]));
    data->time_to_eat = to_microseconds(ft_atoi(argv[3]));
    data->time_to_sleep = to_microseconds(ft_atoi(argv[4]));
    if (argv[5])
        data->num_time_to_eat = ft_atoi(argv[5]);
}

