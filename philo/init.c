/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:19:14 by iduman            #+#    #+#             */
/*   Updated: 2025/09/04 20:19:14 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_monitor(t_monitor *monitor, int argc, char *argv[])
{
	monitor->p_num = ft_atoi(argv[1]);
	monitor->die_time = ft_atoi(argv[2]);
	monitor->eat_time = ft_atoi(argv[3]);
	monitor->sleep_time = ft_atoi(argv[4]);
	monitor->die = ALIVE;
	if (argc == 6)
		monitor->eat_limit = ft_atoi(argv[5]);
	else
		monitor->eat_limit = -1;
	if (monitor->p_num <= 0 || monitor->die_time <= 0
		|| monitor->eat_time <= 0 || monitor->sleep_time <= 0)
		return (0);
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
		return (0);
	return (1);
}

int	init_mutex(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->p_num)
	{
		if (pthread_mutex_init(&monitor->philos[i].left_fork, NULL) != 0)
			return (cleanup_mutexes(monitor, i, 0, 0), 1);
		monitor->philos[i].right_fork = &monitor->philos[
			(i + 1) % monitor->p_num].left_fork;
		i++;
	}
	if (pthread_mutex_init(&monitor->print_mutex, NULL) != 0)
		return (cleanup_mutexes(monitor, i, 0, 0), 1);
	if (pthread_mutex_init(&monitor->dead_mutex, NULL) != 0)
		return (cleanup_mutexes(monitor, i, 1, 0), 1);
	return (0);
}

static void	init_philos(t_monitor *monitor)
{
	int	i;

	i = 0;
	gettimeofday(&monitor->start_time, NULL);
	while (i < monitor->p_num)
	{
		monitor->philos[i].id = i + 1;
		monitor->philos[i].eat_count = 0;
		monitor->philos[i].last_eat = get_time(monitor->start_time);
		monitor->philos[i].monitor = monitor;
		monitor->philos[i].die = ALIVE;
		i++;
	}
}

static void	join_threads(t_monitor *monitor, int c)
{
	int	i;

	i = 0;
	while (i < c)
	{
		pthread_join(monitor->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor->monitor_thread, NULL);
}

void	init_philos_threads(t_monitor *monitor)
{
	int	c;

	c = 0;
	init_philos(monitor);
	while (c < monitor->p_num)
	{
		if (pthread_create(&monitor->philos[c].thread,
				NULL, philo_routine, &monitor->philos[c]) != 0)
		{
			monitor->die = DIE;
			break ;
		}
		c++;
	}
	if (monitor->die == DIE)
		printf("Failed to create threads\n");
	pthread_create(&monitor->monitor_thread, NULL,
		monitor_routine, monitor);
	join_threads(monitor, c);
}
