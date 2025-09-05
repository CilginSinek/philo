/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 21:27:28 by iduman            #+#    #+#             */
/*   Updated: 2025/09/04 21:27:28 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <semaphore.h>
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
    pid_t				pid;
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
    sem_t			*forks;
    sem_t			*print_sem;
    sem_t			*dead_sem;
	t_philo			*philos;
}	t_monitor;


#endif