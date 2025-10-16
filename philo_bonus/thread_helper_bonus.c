/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_helper_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:26:43 by iduman            #+#    #+#             */
/*   Updated: 2025/09/29 21:44:20 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*dead_monitor(void *arg)
{
	t_philo		*philo;
	long int	time_since_eat;
	int			is_alive;

	philo = (t_philo *)arg;
	is_alive = 1;
	while (is_alive)
	{
		usleep(1000);
		sem_wait(philo->eat_mutex);
		is_alive = (philo->die == ALIVE);
		time_since_eat = get_time(philo->monitor->start_time) - philo->last_eat;
		if (time_since_eat > philo->monitor->die_time)
		{
			philo->die = DIE;
			philo->monitor->die = DIE;
			sem_post(philo->eat_mutex);
			sem_wait(philo->monitor->print_sem);
			printf("%ld %d died\n", get_time(philo->monitor->start_time),
				philo->id);
			return (NULL);
		}
		sem_post(philo->eat_mutex);
	}
	return (NULL);
}

int	get_time_six(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

static int	init_eat_mutex(t_philo *philo)
{
	philo->eat_sem_name = init_name("/eat_mutex", get_time_six() + philo->id);
	if (!philo->eat_sem_name)
		return (0);
	philo->eat_mutex = sem_open(philo->eat_sem_name, O_CREAT, 0644, 1);
	if (philo->eat_mutex == SEM_FAILED)
	{
		free(philo->eat_sem_name);
		return (0);
	}
	return (1);
}

void	init_philosopher_routine(t_philo *philo, pthread_t *dead_thread)
{
	if (!init_eat_mutex(philo))
	{
		philo->die = DIE;
		print_action(philo, "semaphore init failed");
		cleanup_child(philo->monitor, philo);
		exit(1);
	}
	sem_wait(philo->eat_mutex);
	philo->last_eat = get_time(philo->monitor->start_time);
	sem_post(philo->eat_mutex);
	pthread_create(dead_thread, NULL, dead_monitor, philo);
	if (philo->id % 2 == 0)
		usleep(philo->monitor->eat_time * 500);
}
