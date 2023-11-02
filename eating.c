/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnyamets <lnyamets@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 23:08:37 by lnyamets          #+#    #+#             */
/*   Updated: 2023/11/02 03:29:44 by lnyamets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	eating_simulation(t_philosophe *p_philosophe, SharedInfo *shared_info)
{
	pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), L_FORK_BHVR, shared_info);
	pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id
		% p_philosophe->params->num_philosophes]);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), R_FORK_BHVR, shared_info);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), EAT_BHVR, shared_info);
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
