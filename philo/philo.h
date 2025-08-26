#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo
{
	int id;
	struct timeval	last_eat;
	void			*right_fork;
	void			*left_fork;
}   t_philo;

typedef enum e_is_die
{
	DIE = 1,
	ALIVE
}   t_is_die;

typedef struct s_monitor
{
	int			p_num;
	int			die_time;
	int			eat_time;
	int			sleep_time;
	int			eat_limit;
	t_is_die	die;
}	t_monitor;

int	ft_atoi(const char *str);

#endif