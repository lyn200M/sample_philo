/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnyamets <lnyamets@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 21:47:15 by lnyamets          #+#    #+#             */
/*   Updated: 2023/11/01 23:11:50 by lnyamets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	philosopher_simulation(t_philosophe *p_all_philosophe
	, t_simulation_params *simu_parametters)
{
	int	index;

	index = 0;
	while (1)
	{
		if (p_all_philosophe[index].params->total_meals_eaten
			== p_all_philosophe[index].params->num_philosophes)
		{
			destroy_and_free(p_all_philosophe, simu_parametters);
			return ;
		}
		if ((get_current_time_in_ms() - p_all_philosophe[index].last_meal_time)
			> (unsigned long)simu_parametters->time_to_die)
		{
			usleep(100);
			pthread_mutex_lock(p_all_philosophe->print_behavior_mutex);
			printf("%lums	philosopher: %d died :(\n", elapsed_time
				(p_all_philosophe), p_all_philosophe->id);
			destroy_and_free(p_all_philosophe, simu_parametters);
			return ;
		}
		index = (index + 1) % simu_parametters->num_philosophes;
		usleep(500);
	}
}

void	*philosopher_behavior(void *arg)
{
	t_philosophe	*p_philosophe;

	p_philosophe = (t_philosophe *)arg;
	if (p_philosophe->id % 2 == 0)
		usleep(10);
	p_philosophe->last_meal_time = get_current_time_in_ms();
	while (1)
	{
		eating_simulation(p_philosophe);
		p_philosophe->meals_to_eaten++;
		if (p_philosophe->meals_to_eaten == p_philosophe->params->meals_to_eat)
			p_philosophe->params->total_meals_eaten++;
		usleep(p_philosophe->params->time_to_eat * 1000);
		p_philosophe->last_meal_time = get_current_time_in_ms();
		pthread_mutex_unlock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
		pthread_mutex_unlock(&p_philosophe->fork_mutex[p_philosophe->id
			% p_philosophe->params->num_philosophes]);
		print_behavior(p_philosophe, elapsed_time(p_philosophe), SLEEP_BHVR);
		usleep(p_philosophe->params->time_to_sleep * 1000);
		print_behavior(p_philosophe, elapsed_time(p_philosophe), THINK_BHVR);
	}
	return (0);
}

int	init_philosophe(t_philosophe *p_all_philosophe,
	t_simulation_params *simu_parametters)
{
	pthread_mutex_t	*p_mutex;
	pthread_mutex_t	*print_bahavior_mutex;
	int				i;

	p_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* simu_parametters->num_philosophes);
	if (!p_all_philosophe || !p_mutex)
		return (free_data_struct(p_all_philosophe, simu_parametters, p_mutex));
	i = 0;
	while (i < simu_parametters->num_philosophes)
	{
		if (pthread_mutex_init(&p_mutex[i], 0))
			return (free_data_struct(p_all_philosophe, simu_parametters,
					p_mutex));
		i++;
	}
	print_bahavior_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(print_bahavior_mutex, 0))
		return (free_data_struct(p_all_philosophe, simu_parametters, p_mutex));
	initialisation(p_all_philosophe, simu_parametters, p_mutex,
		print_bahavior_mutex);
	return (0);
}

int	init_simulation_params(t_simulation_params *params, char **av)
{
	if (is_good_args(av))
		return (log_error());
	params->total_meals_eaten = 0;
	params->num_philosophes = ft_atoi(av[1]);
	params->time_to_die = ft_atoi(av[2]);
	params->time_to_eat = ft_atoi(av[3]);
	params->time_to_sleep = ft_atoi(av[4]);
	params->start_time = get_current_time_in_ms();
	if (params->num_philosophes <= 0 || params->time_to_die <= 0 || params
		->time_to_eat <= 0 || params->time_to_sleep <= 0)
		return (1);
	if (av[5] && ft_atoi(av[5]) > 0)
		params->meals_to_eat = ft_atoi(av[5]);
	else if (av[5] && ft_atoi(av[5]) <= 0)
		return (1);
	else
		params->meals_to_eat = -1;
	return (0);
}

int	main(int ac, char **av)
{
	t_simulation_params	*s_prm;
	t_philosophe		*p_all_philosophe;
	pthread_t			*p_threads;
	int					i;

	if (ac != 5 && ac != 6)
		return (log_error());
	s_prm = (t_simulation_params *)malloc(sizeof(t_simulation_params));
	if (init_simulation_params(s_prm, av))
		return (free_data_struct(0, s_prm, 0));
	p_all_philosophe = (t_philosophe *)malloc(sizeof(t_philosophe)
			* s_prm->num_philosophes);
	if (init_philosophe(p_all_philosophe, s_prm))
		return (1);
	p_threads = (pthread_t *)malloc(sizeof(pthread_t) * s_prm->num_philosophes);
	i = -1;
	while (++i < p_all_philosophe->params->num_philosophes)
	{
		pthread_create(&p_threads[i], NULL, philosopher_behavior,
			p_all_philosophe + i);
		usleep(100);
	}
	philosopher_simulation(p_all_philosophe, s_prm);
	free(p_threads);
	return (0);
}
