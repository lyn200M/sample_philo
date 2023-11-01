#include "philosopher.h"


int is_pair(int nbr)
{
    if (nbr % 2 == 0)
        return (1);
    return (0);
}

void    destroy_fork_mutexs(t_philosophe *p_all_philosophe)
{
    int index;

    index = 0;
    while (index < p_all_philosophe->params->num_philosophes)
    {
        pthread_mutex_destroy(&p_all_philosophe->fork_mutex[index++]);
    }

}

void    print_behavior(t_philosophe *p_philosophe, unsigned long time, char *bahavior_msg)
{
    pthread_mutex_lock(p_philosophe->print_behavior_mutex);
	printf("%lums	philosopher: %d %s\n", time, p_philosophe->id, bahavior_msg);
	pthread_mutex_unlock(p_philosophe->print_behavior_mutex);
}

unsigned long elapsed_time(t_philosophe *p_philosophe)
{
    return (get_current_time_in_ms() - p_philosophe->params->start_time);
}

void    philosopher_simulation(t_philosophe *p_all_philosophe, t_simulation_params *simu_parametters)
{
    int index;

    index = 0;
    while (1)
    {
        if (p_all_philosophe[index].params->total_meals_eaten == p_all_philosophe[index].params->num_philosophes)
        {
            destroy_fork_mutexs(p_all_philosophe);
            free_data_struct(p_all_philosophe, simu_parametters, p_all_philosophe->fork_mutex);
            return;
        }
        if ((get_current_time_in_ms() - p_all_philosophe[index].last_meal_time)
         > (unsigned long) simu_parametters->time_to_die)
         {
            usleep(100);
            pthread_mutex_lock(p_all_philosophe->print_behavior_mutex);
            printf("%lums	philosopher: %d DIED :(\n", elapsed_time(p_all_philosophe), p_all_philosophe->id);
            destroy_fork_mutexs(p_all_philosophe);
            return;
         }
         index = (index + 1) % simu_parametters->num_philosophes;
         usleep(500);
    }
}

void    *philosopher_behavior(void  *arg)
{
    t_philosophe    *p_philosophe;

    p_philosophe = (t_philosophe *)arg;
    if(is_pair(p_philosophe->id))
        usleep (15);
    p_philosophe->last_meal_time = get_current_time_in_ms();
    while (1)
    {
        pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
        print_behavior(p_philosophe, elapsed_time(p_philosophe), LEFT_FORK_BEHAVIOR);
        pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id % p_philosophe->params->num_philosophes]);
        print_behavior(p_philosophe, elapsed_time(p_philosophe), RIGHT_FORK_BEHAVIOR);
        print_behavior(p_philosophe, elapsed_time(p_philosophe), EAT_BEHAVIOR);
        p_philosophe->meals_to_eaten++;
        if (p_philosophe->meals_to_eaten == p_philosophe->params->meals_to_eat)
            p_philosophe->params->total_meals_eaten++;
        usleep(p_philosophe->params->time_to_eat * 1000);
        p_philosophe->last_meal_time = get_current_time_in_ms();
        pthread_mutex_unlock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
		pthread_mutex_unlock(&p_philosophe->fork_mutex[p_philosophe->id % p_philosophe->params->num_philosophes]);
		print_behavior(p_philosophe, elapsed_time(p_philosophe), SLEEP_BEHAVIOR);
		usleep(p_philosophe->params->time_to_sleep * 1000);
		print_behavior(p_philosophe, elapsed_time(p_philosophe), THINK_BEHAVIOR);
    }
    return (0);
}

int init_philosophe(t_philosophe *p_all_philosophe, t_simulation_params *simu_parametters)
{
    pthread_mutex_t     *p_mutex;
    pthread_mutex_t     print_bahavior_mutex;
    int                 i;

    p_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * simu_parametters->num_philosophes);
    if(!p_all_philosophe || !p_mutex)
        return (free_data_struct(p_all_philosophe, simu_parametters, p_mutex));
    i = 0;
    while (i < simu_parametters->num_philosophes)
    {
        if (pthread_mutex_init(&p_mutex[i], 0))
            return (free_data_struct(p_all_philosophe, simu_parametters, p_mutex));
        i++;
    }
    if (pthread_mutex_init(&print_bahavior_mutex, 0))
        return (free_data_struct(p_all_philosophe, simu_parametters, p_mutex));
    i = 0;
    while (i < simu_parametters->num_philosophes)
    {
        p_all_philosophe[i].id = i + 1;
        p_all_philosophe[i].meals_to_eaten = 0;
        p_all_philosophe[i].fork_mutex = p_mutex;
        p_all_philosophe[i].params = simu_parametters;
        p_all_philosophe[i].print_behavior_mutex = &print_bahavior_mutex;
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
    params->total_meals_eaten = 0;
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
    pthread_t           *p_threads;
    int i;

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
    p_threads = (pthread_t *)malloc(sizeof(pthread_t) * simu_parametters->num_philosophes);
    i = 0;
    while (i < p_all_philosophe->params->num_philosophes)
    {
        pthread_create(&p_threads[i], NULL, philosopher_behavior, p_all_philosophe + i);
        i++;
        usleep(100);
    }
    philosopher_simulation(p_all_philosophe, simu_parametters);
    free(p_threads);

    return(0);
}
