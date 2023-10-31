#include "philosopher.h"

int init_philosophe(t_philosophe *p_all_philosophe, t_simulation_params *simu_parametters)
{
    pthread_mutex_t     *p_mutex;
    int                 i;

    p_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * simu_parametters->num_philosophes);
    if(!p_all_philosophe || !p_mutex)
        return (free_data_struct(p_all_philosophe, simu_parametters, p_mutex));
    i = 0;
    while (i < simu_parametters->num_philosophes)
    {
        if (pthread_mutex_init(&p_mutex[i], NULL))
            return (free_data_struct(p_all_philosophe, simu_parametters, p_mutex));
        i++;
    }
    i = 0;
    while (i < simu_parametters->num_philosophes)
    {
        p_all_philosophe[i].id = i + 1;
        p_all_philosophe[i].meals_to_eaten = 0;
        p_all_philosophe[i].mutex = p_mutex;
        p_all_philosophe[i].params = simu_parametters;
        //printf("p_all_philosophe->id = %d\np_all_philosophe->meals_to_eaten = %d\np_all_philosophe->mutex = %p\n", 
       //p_all_philosophe[i].id, p_all_philosophe[i].meals_to_eaten, p_all_philosophe[i].mutex);
        i++;
    }
    return (0);
}

int init_sinulation_params(t_simulation_params *params, char **av)
{
    if (is_good_args(av))
        return (log_error());
    params->num_philosophes = ft_atoi(av[1]);
    params->time_to_die = ft_atoi(av[2]);
    params->time_to_eat = ft_atoi(av[3]);
    params->time_to_sleep = ft_atoi(av[4]);
    params->start_time = get_current_time_in_ms();
    if (params->num_philosophes <= 0 || params->time_to_die <= 0 || params->time_to_eat <= 0
        || params->time_to_sleep <= 0)
        return(1); // todo return a speciphique error for this case
    if (av[5] && ft_atoi(av[5]) > 0)
        params->meals_to_eat = ft_atoi(av[5]);
    else if (av[5] && ft_atoi(av[5]) <= 0)
        return (1); //todo cas d'erreur a gerer specifiquement
    else
        params->meals_to_eat = -1; // todo a revoir cette valeur de -1 si valable ou pas
    return (0);
}

int main(int ac, char **av)
{
    t_simulation_params *simu_parametters;
    t_philosophe        *p_all_philosophe;

    if (ac != 5 && ac != 6)
        return(log_error());
    simu_parametters = (t_simulation_params *) malloc(sizeof(t_simulation_params));
    if (init_sinulation_params(simu_parametters, av))
        return (free_data_struct(0, simu_parametters, 0));
    //printf("params->num_philosophes = %d\nparams->time_to_die = %d\nparams->time_to_eat = %d\nparams->time_to_sleep = %d\nparams->start_time = %lu\nparams->meals_to_eat = %d\n", 
       // simu_parametters->num_philosophes, simu_parametters->time_to_die, simu_parametters->time_to_eat, simu_parametters->time_to_sleep, simu_parametters->start_time, simu_parametters->meals_to_eat);
    p_all_philosophe = (t_philosophe *)malloc(sizeof(t_philosophe) * simu_parametters->num_philosophes);
    if(init_philosophe(p_all_philosophe, simu_parametters))
        return (1); // todo creer un msg de long plus explicit avant le return
    return(0);
}