/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 21:27:28 by iduman            #+#    #+#             */
/*   Updated: 2025/09/29 06:36:15 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>

typedef enum e_is_die
{
	DIE = 1,
	ALIVE
}	t_is_die;

typedef enum e_boolean
{
	NONE = -1,
	FALSE = 0,
	TRUE = 1
}	t_boolean;

typedef struct s_philo
{
	int					id;
	int					eat_count;
	long int			last_eat;
	pid_t				pid;
	struct s_monitor	*monitor;
	int					forks;
	int					print;
	sem_t				*eat_mutex;
	char 				*eat_sem_name;
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
	sem_t			*start_sem;
	sem_t			*forks;
	sem_t			*print_sem;
	sem_t			*eat_sems;
	char			*sem_names[4];
	t_boolean		eat_complete;
	t_philo			*philos;
}	t_monitor;

//Libft
int			ft_strlen(const char *str);
int			ft_atoi(const char *str);
char		*ft_strdup(const char *str);
char		*ft_itoa(int n);
int			is_all_numaric(int argc, char **argv);
//Print
long int	get_time(struct timeval start);
void		print_action(t_philo *philo, char *action);
//Init
int			init_monitor(t_monitor *monitor, int argc, char *argv[]);
void		cleanup_semaphores(t_monitor *monitor, int *flags);
void		cleanup_child(t_monitor *monitor);
int			init_semaphores(t_monitor *monitor);
int			init_philos(t_monitor *monitor);
char		*init_name(char *base, int id);
int			get_time_six(void);
void free_names(t_monitor *monitor);
//Philos
void		*dead_monitor(void *arg);
void		start_flag_up(t_monitor *monitor);
int			healty_check(t_philo *philo);
int			take_forks(t_philo *philo);
int			is_alive_in_event(t_philo *philo, int event_time);
void		create_start_process(t_monitor *monitor);
void		philosopher_routine(t_philo *philo);

#endif