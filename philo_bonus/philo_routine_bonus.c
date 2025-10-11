/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:29:51 by iduman            #+#    #+#             */
/*   Updated: 2025/10/11 18:39:55 by iduman           ###   ########.fr       */
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
		print_action(philo, "died");
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
	usleep(200);
	return (0);
}

void	philosopher_routine(t_philo *philo)
{
	pthread_t	dead_thread;

	gettimeofday(&philo->monitor->start_time, NULL);
	init_philosopher_routine(philo, &dead_thread);
	while (philo->die == ALIVE)
	{
		if (feed_philo(philo)
			|| sleep_philo(philo) || think_philo(philo))
			break ;
	}
	pthread_join(dead_thread, NULL);
	if (philo->forks)
		while (philo->forks--)
			sem_post(philo->monitor->forks);
	cleanup_child(philo->monitor, philo);
	exit(0);
}
