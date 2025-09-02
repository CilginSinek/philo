#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo
{
	int 			id;
	int				eat_count;
	long int		last_eat;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	left_fork;
	pthread_t		thread;
}   t_philo;

typedef enum e_is_die
{
	DIE = 1,
	ALIVE
}   t_is_die;

typedef struct s_monitor
{
	int				p_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_limit;
	t_is_die		die;
	struct timeval	start_time;
	pthread_mutex_t	print_mutex;
	t_philo			*philos;
}	t_monitor;

int	ft_atoi(const char *str);

#endif