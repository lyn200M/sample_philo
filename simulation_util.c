/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_util.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnyamets <lnyamets@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 21:46:56 by lnyamets          #+#    #+#             */
/*   Updated: 2023/11/01 22:54:30 by lnyamets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	destroy_fork_mutexs(t_philosophe *p_all_philosophe)
{
	int	index;

	index = 0;
	while (index < p_all_philosophe->params->num_philosophes)
	{
		pthread_mutex_destroy(&(p_all_philosophe->fork_mutex[index]));
		index++;
	}
	pthread_mutex_destroy(p_all_philosophe->print_behavior_mutex);
}

void	print_behavior(t_philosophe *p_philosophe, unsigned long time,
	char *bahavior_msg)
{
	pthread_mutex_lock(p_philosophe->print_behavior_mutex);
	printf("%lums	philosopher: %d %s\n", time, p_philosophe->id,
		bahavior_msg);
	pthread_mutex_unlock(p_philosophe->print_behavior_mutex);
}

unsigned long	elapsed_time(t_philosophe *p_philosophe)
{
	return (get_current_time_in_ms() - p_philosophe->params->start_time);
}

void	destroy_and_free(t_philosophe *p_all_philosophe,
	t_simulation_params *simu_parametters)
{
	destroy_fork_mutexs(p_all_philosophe);
	free_data_struct(p_all_philosophe, simu_parametters,
		p_all_philosophe->fork_mutex);
}

void	initialisation(t_philosophe *p_all_philosophe, t_simulation_params *
		simu_parametters, pthread_mutex_t *p_mutex,
			pthread_mutex_t *print_bahavior_mutex)
{
	int	i;

	i = 0;
	while (i < simu_parametters->num_philosophes)
	{
		p_all_philosophe[i].id = i + 1;
		p_all_philosophe[i].meals_to_eaten = 0;
		p_all_philosophe[i].fork_mutex = p_mutex;
		p_all_philosophe[i].params = simu_parametters;
		p_all_philosophe[i].print_behavior_mutex = print_bahavior_mutex;
		i++;
	}
}
