/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:30:36 by iduman            #+#    #+#             */
/*   Updated: 2025/09/04 20:30:36 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	get_time(struct timeval start)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec - start.tv_sec) * 1000 + (
			current.tv_usec - start.tv_usec) / 1000);
}

void	print_events(pthread_mutex_t *print_mutex, int id,
	char *str, struct timeval start)
{
	pthread_mutex_lock(print_mutex);
	printf("%ld %d %s\n", get_time(start), id, str);
	pthread_mutex_unlock(print_mutex);
}

int	dead_check(t_monitor *monitor)
{
	pthread_mutex_lock(&monitor->dead_mutex);
	if (monitor->die == DIE)
	{
		pthread_mutex_unlock(&monitor->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&monitor->dead_mutex);
	return (0);
}

int	is_full(t_monitor *monitor)
{
	int	i;

	i = 0;
	if (monitor->eat_limit == -1)
		return (0);
	while (i < monitor->p_num)
	{
		if (monitor->philos[i].eat_count < monitor->eat_limit)
			return (0);
		i++;
	}
	monitor->die = DIE;
	return (1);
}

void	cleanup_mutexes(t_monitor *monitor, int fork_count,
		int print_init, int dead_init)
{
	int	i;

	i = 0;
	while (i < fork_count)
		pthread_mutex_destroy(&monitor->philos[i++].left_fork);
	if (print_init)
		pthread_mutex_destroy(&monitor->print_mutex);
	if (dead_init)
		pthread_mutex_destroy(&monitor->dead_mutex);
}
