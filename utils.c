#include "philo.h"

// Current time in milliseconds 1 sec == 1000 milliseconds == 1000000 microseconds
//curr time + time to eat = finish eating, curr time + time to die = die.
size_t	get_current_time(void)
{
	struct timeval	time;
	if (gettimeofday(&time, NULL) == -1)
		ft_putstr_fd("gettimeofday() error\n", 2);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int ft_atoi(char *str)
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