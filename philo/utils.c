/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:30:36 by iduman            #+#    #+#             */
/*   Updated: 2025/09/28 16:30:49 by iduman           ###   ########.fr       */
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
	long	time;

	time = get_time(start);
	pthread_mutex_lock(print_mutex);
	printf("%ld %d %s\n", time, id, str);
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
		pthread_mutex_lock(&monitor->philos[i].eat_mutex);
		if (monitor->philos[i].eat_count < monitor->eat_limit)
		{
			pthread_mutex_unlock(&monitor->philos[i].eat_mutex);
			return (0);
		}
		pthread_mutex_unlock(&monitor->philos[i].eat_mutex);
		i++;
	}
	pthread_mutex_lock(&monitor->dead_mutex);
	monitor->die = DIE;
	pthread_mutex_unlock(&monitor->dead_mutex);
	return (1);
}

/**
 * @brief Cleans up mutexes and frees allocated memory for philosophers.
 * @param monitor Pointer to the monitor structure.
 * @param args An array of three integers
 * @param 0 : Number of philosopher's left_fork mutexes have been initialized.
 * @param 1 : Flag indicating if print_mutex has been initialized true/false.
 * @param 2 : Flag indicating if dead_mutex has been initialized true/false.
 * @param 3 : Number of philosopher's eat_mutex mutexes have been initialized.
 * @return void
 */
void	cleanup_mutexes(t_monitor *monitor, int *args)
{
	int	i;

	i = 0;
	while (i < args[0])
		pthread_mutex_destroy(&monitor->philos[i++].left_fork);
	if (args[1])
		pthread_mutex_destroy(&monitor->print_mutex);
	if (args[2])
		pthread_mutex_destroy(&monitor->dead_mutex);
	i = 0;
	while (i < args[3])
		pthread_mutex_destroy(&monitor->philos[i++].eat_mutex);
	free(monitor->philos);
}
