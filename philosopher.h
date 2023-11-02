/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnyamets <lnyamets@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 21:39:12 by lnyamets          #+#    #+#             */
/*   Updated: 2023/11/02 04:38:19 by lnyamets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# define L_FORK_BHVR "has taken a fork"
# define R_FORK_BHVR "has taken a fork"
# define EAT_BHVR "is eating"
# define SLEEP_BHVR "is sleeping"
# define THINK_BHVR "is thinking"

typedef struct s_simulation_params
{
	int				num_philosophes;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_to_eat;
	int				total_meals_eaten;
	unsigned long	start_time;
}					t_simulation_params;

typedef struct s_philosophe
{
	int					id;
	int					meals_to_eaten;
	unsigned long		last_meal_time;
	pthread_mutex_t		*fork_mutex;
	pthread_mutex_t		*print_behavior_mutex;
	t_simulation_params	*params;
}						t_philosophe;

typedef struct s_sharedInfo
{
	int	philosopher_died;
}		t_sharedInfo;

typedef struct s_threadArgs
{
	t_philosophe	*p_philosophe;
	t_sharedInfo	*shared_info;
}					t_threadArgs;

int				log_error(void);
int				init_simulation_params(t_simulation_params *params, char **av);
int				is_good_args(char **av);
int				ft_atoi(char *str);
unsigned long	get_current_time_in_ms(void);
int				free_data_struct(t_philosophe *p_all_philosophe,
					t_simulation_params *simu_parametters,
					pthread_mutex_t *p_mutex);
int				init_philosophe(t_philosophe *p_all_philosophe,
					t_simulation_params *simu_parametters);
void			destroy_fork_mutexs(t_philosophe *p_all_philosophe);
void			print_behavior(t_philosophe *p_philosophe, unsigned long time,
					char *bahavior_msg, t_sharedInfo *shared_info);
unsigned long	elapsed_time(t_philosophe *p_philosophe);
void			destroy_and_free(t_philosophe *p_all_philosophe,
					t_simulation_params *simu_parametters,
					t_threadArgs *thread_args, pthread_t *p_threads);
void			initialisation(t_philosophe *p_all_philosophe,
					t_simulation_params *simu_parametters,
					pthread_mutex_t *p_mutex,
					pthread_mutex_t *print_bahavior_mutex);
void			eating_simulation(t_philosophe *p_philosophe,
					t_sharedInfo *shared_info);
void			join_threads(pthread_t *p_threads, char **av);
void			*philosopher_behavior(void *arg);
void			create_philosopher_threads(t_philosophe *p_all_philosophe,
					pthread_t *p_threads, t_threadArgs *thread_args,
					t_sharedInfo *shared_info);
void			one_philosopher_behavior(t_philosophe *p_philosophe,
					t_sharedInfo *shared_info);
void			multi_philosophers_behavior(t_philosophe *p_philosophe,
					t_sharedInfo *shared_info);
#endif
