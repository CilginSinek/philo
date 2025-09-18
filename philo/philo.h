/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:45:56 by iduman            #+#    #+#             */
/*   Updated: 2025/09/04 20:45:56 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

typedef enum e_is_die
{
	DIE = 1,
	ALIVE
}	t_is_die;

typedef struct s_philo
{
	int					id;
	int					eat_count;
	long int			last_eat;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		left_fork;
	pthread_t			thread;
	struct s_monitor	*monitor;
	t_is_die			die;
}	t_philo;

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
	pthread_mutex_t	dead_mutex;
	pthread_t		monitor_thread;
	t_philo			*philos;
}	t_monitor;

int			is_all_numaric(int argc, char **argv);
int			ft_atoi(const char *str);
long int	get_time(struct timeval start);
void		print_events(pthread_mutex_t *print_mutex, int id,
				char *str, struct timeval start);
int			dead_check(t_monitor *monitor);
int			is_full(t_monitor *monitor);
int			is_alive_in_event(t_philo *philo, int event_time);

int			init_monitor(t_monitor *monitor, int argc, char *argv[]);
int			init_mutex(t_monitor *monitor);
void		init_philos_threads(t_monitor *monitor);
void		cleanup_mutexes(t_monitor *monitor, int fork_count,
				int print_init, int dead_init);

int			feed_philo(t_monitor *monitor, t_philo *philo);
int			sleep_philo(t_monitor *monitor, t_philo *philo);
int			think_philo(t_monitor *monitor, t_philo *philo);

void		*monitor_routine(void *arg);
void		*philo_routine(void *arg);

#endif