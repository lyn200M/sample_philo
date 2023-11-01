/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnyamets <lnyamets@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 23:08:37 by lnyamets          #+#    #+#             */
/*   Updated: 2023/11/01 23:11:10 by lnyamets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	eating_simulation(t_philosophe *p_philosophe)
{
	pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id - 1]);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), L_FORK_BHVR);
	pthread_mutex_lock(&p_philosophe->fork_mutex[p_philosophe->id
		% p_philosophe->params->num_philosophes]);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), R_FORK_BHVR);
	print_behavior(p_philosophe, elapsed_time(p_philosophe), EAT_BHVR);
}
