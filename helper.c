#include "philosopher.h"

int log_error(void)
{
    printf("Param Error\n");
    return (1);
}

int is_good_args(char **av)
{
    int i;
    int j;

    i = 1;
    while (av[i])
    {
        j = 0;
        while (av[i][j])
        {
            if ((av[i][j] < '0' || av[i][j] > '9') && j != 0)
                return (1); //todo bien logger l'erreur et son emplacement
            j++;
        }
        i++;
    }
    return (0);
}

int ft_atoi(char *str)
{
    int i;
    int result;
    int sign;

    i = 0;
    result = 0;
    sign = 1;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
    if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
            sign *= -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
	{
        result = (result * 10) + str[i] - '0';
        i++;
    }
    return (result * sign);
}

unsigned long   get_current_time_in_ms(void)
{
    struct  timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int free_data_struct(t_philosophe *p_all_philosophe, t_simulation_params *simu_parametters, pthread_mutex_t *p_mutex)
{
    if (simu_parametters)
        free(simu_parametters);
    if (p_all_philosophe)
        free(p_all_philosophe);
    if (p_mutex)
        free(p_mutex);
    return (1);
}