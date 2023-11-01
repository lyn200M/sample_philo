#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# define LEFT_FORK_BEHAVIOR "has taken a left fork"
# define RIGHT_FORK_BEHAVIOR "has taken a right fork"
# define EAT_BEHAVIOR "is eat pasta"
# define SLEEP_BEHAVIOR "is sleeping zzz"
# define THINK_BEHAVIOR "is thinking"

typedef struct s_simulation_params
{
    int             num_philosophes;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             meals_to_eat;
    int             total_meals_eaten;
    unsigned long   start_time;
}                   t_simulation_params;

typedef struct  s_philosophe
{
    int                 id;
    int                 meals_to_eaten;
    unsigned long       last_meal_time;
    pthread_mutex_t     *fork_mutex;
    pthread_mutex_t     *print_behavior_mutex;
    t_simulation_params *params;
}                       t_philosophe;

int log_error(void);
int init_sinulation_params(t_simulation_params *params, char **av);
int is_good_args(char **av);
int ft_atoi(char *str);
unsigned long   get_current_time_in_ms(void);
int free_data_struct(t_philosophe *p_all_philosophe, t_simulation_params *simu_parametters, pthread_mutex_t *p_mutex);
int init_philosophe(t_philosophe *p_all_philosophe, t_simulation_params *simu_parametters);
void    destroy_fork_mutexs(t_philosophe *p_all_philosophe);
#endif

