/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:29:51 by iduman            #+#    #+#             */
/*   Updated: 2025/09/29 06:33:15 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	healty_check(t_philo *philo)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	if (monitor->die == DIE)
		return (1);
	sem_wait(philo->eat_mutex);
	if (get_time(monitor->start_time) - philo->last_eat > monitor->die_time)
	{
		philo->die = DIE;
		monitor->die = DIE;
		print_action(philo, "is died");
		sem_post(philo->eat_mutex);
		return (1);
	}
	sem_post(philo->eat_mutex);
	return (0);
}

static int	feed_philo(t_philo *philo)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	if (take_forks(philo))
		return (1);
	sem_wait(philo->eat_mutex);
	philo->last_eat = get_time(monitor->start_time);
	sem_post(philo->eat_mutex);
	print_action(philo, "is eating");
	usleep(monitor->eat_time * 1000);
	sem_wait(philo->eat_mutex);
	philo->eat_count++;
	sem_post(philo->eat_mutex);
	if (monitor->eat_complete != NONE && philo->eat_count == monitor->eat_limit)
		sem_post(monitor->eat_sems);
	sem_post(monitor->forks);
	sem_post(monitor->forks);
	philo->forks = 0;
	return (0);
}

static int	sleep_philo(t_philo *philo)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	if (healty_check(philo))
		return (1);
	print_action(philo, "is sleeping");
	if (is_alive_in_event(philo, monitor->sleep_time))
		return (1);
	return (0);
}

static int	think_philo(t_philo *philo)
{
	if (healty_check(philo))
		return (1);
	print_action(philo, "is thinking");
	return (0);
}

int init_eat_mutex(t_philo *philo)
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

void clear_eat_mutex(t_philo *philo)
{
	if (philo->eat_mutex)
	{
		sem_close(philo->eat_mutex);
		sem_unlink(philo->eat_sem_name);
	}
	if (philo->eat_sem_name)
		free(philo->eat_sem_name);
}

void	philosopher_routine(t_philo *philo)
{
	pthread_t	dead_thread;

	if(!init_eat_mutex(philo))
	{
		philo->die = DIE;
		print_action(philo, "semaphore init failed");
		cleanup_child(philo->monitor);
		exit(1);
	}
	sem_wait(philo->eat_mutex);
	philo->last_eat = get_time(philo->monitor->start_time);
	sem_post(philo->eat_mutex);
	pthread_create(&dead_thread, NULL, dead_monitor, philo);
	if (philo->id % 2 == 0)
		usleep(philo->monitor->eat_time * 500);
	while (philo->die == ALIVE)
	{
		if (healty_check(philo) || feed_philo(philo)
			|| sleep_philo(philo) || think_philo(philo))
			break ;
	}
	pthread_join(dead_thread, NULL);
	if (philo->forks)
	{
		while (philo->forks)
		{
			sem_post(philo->monitor->forks);
			philo->forks--;
		}
	}
	cleanup_child(philo->monitor);
	clear_eat_mutex(philo);
	exit(0);
}
