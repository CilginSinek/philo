/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:28:31 by iduman            #+#    #+#             */
/*   Updated: 2025/09/04 20:28:31 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_forks(t_monitor *monitor, t_philo *philo)
{
	if (dead_check(monitor))
		return ;
	print_events(&monitor->print_mutex, philo->id,
		"has taken a fork", monitor->start_time);
}

static int	take_forks(t_monitor *monitor, t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (dead_check(monitor))
		return (1);
	if (philo->id % 2 == 0)
	{
		first = &philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = &philo->left_fork;
	}
	pthread_mutex_lock(first);
	if (dead_check(monitor))
		return (pthread_mutex_unlock(first), 1);
	print_forks(monitor, philo);
	pthread_mutex_lock(second);
	if (dead_check(monitor))
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 1);
	print_forks(monitor, philo);
	return (0);
}

int	feed_philo(t_monitor *monitor, t_philo *philo)
{
	if (take_forks(monitor, philo))
		return (1);
	print_events(&monitor->print_mutex, philo->id,
		"is eating", monitor->start_time);
	philo->last_eat = get_time(monitor->start_time);
	usleep(monitor->eat_time * 1000);
	philo->last_eat = get_time(monitor->start_time);
	philo->eat_count++;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
	return (0);
}

int	sleep_philo(t_monitor *monitor, t_philo *philo)
{
	if (dead_check(monitor))
		return (1);
	print_events(&monitor->print_mutex, philo->id,
		"is sleeping", monitor->start_time);
	if (is_alive_in_event(philo, monitor->sleep_time))
		return (1);
	return (0);
}

int	think_philo(t_monitor *monitor, t_philo *philo)
{
	if (dead_check(monitor))
		return (1);
	print_events(&monitor->print_mutex, philo->id,
		"is thinking", monitor->start_time);
	usleep(1000);
	return (0);
}
