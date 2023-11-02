/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnyamets <lnyamets@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 23:08:37 by lnyamets          #+#    #+#             */
/*   Updated: 2023/11/02 04:34:27 by lnyamets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	eating_simulation(t_philosophe *p_philosophe, t_sharedInfo *shared_info)
{
	pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), L_FORK_BHVR,
		shared_info);
	pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id
		% p_philosophe->params->num_philosophes]);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), R_FORK_BHVR,
		shared_info);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), EAT_BHVR,
		shared_info);
}

void	join_threads(pthread_t *p_threads, char **av)
{
	int	i;

	i = 0;
	while (i < ft_atoi(av[1]))
	{
		pthread_join(p_threads[i], NULL);
		i++;
	}
}

void	create_philosopher_threads(t_philosophe *p_all_philosophe,
	pthread_t *p_threads, t_threadArgs *thread_args, t_sharedInfo *shared_info)
{
	int	i;

	i = -1;
	while (++i < p_all_philosophe->params->num_philosophes)
	{
		thread_args[i].p_philosophe = p_all_philosophe + i;
		thread_args[i].shared_info = shared_info;
		pthread_create(&p_threads[i], NULL,
			philosopher_behavior, &thread_args[i]);
		usleep(100);
	}
}

void	one_philosopher_behavior(t_philosophe *p_philosophe,
		t_sharedInfo *shared_info)
{
	pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
	print_behavior(p_philosophe, elapsed_time(p_philosophe),
		L_FORK_BHVR, shared_info);
	pthread_mutex_unlock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
}

void	multi_philosophers_behavior(t_philosophe *p_philosophe,
		t_sharedInfo *shared_info)
{
	eating_simulation(p_philosophe, shared_info);
	p_philosophe->meals_to_eaten++;
	if (p_philosophe->meals_to_eaten == p_philosophe->params->meals_to_eat)
		p_philosophe->params->total_meals_eaten++;
	usleep(p_philosophe->params->time_to_eat * 1000);
	p_philosophe->last_meal_time = get_current_time_in_ms();
	pthread_mutex_unlock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
	pthread_mutex_unlock(&p_philosophe->fork_mutex[p_philosophe->id
		% p_philosophe->params->num_philosophes]);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), SLEEP_BHVR,
		shared_info);
	usleep(p_philosophe->params->time_to_sleep * 1000);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), THINK_BHVR,
		shared_info);
}
