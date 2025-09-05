/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:19:11 by iduman            #+#    #+#             */
/*   Updated: 2025/09/03 19:15:36 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	setdead(t_monitor *monitor, int i)
{
	pthread_mutex_lock(&monitor->dead_mutex);
	monitor->die = DIE;
	monitor->philos[i].die = DIE;
	print_events(&monitor->print_mutex, monitor->philos[i].id,
		"is died", monitor->start_time);
	pthread_mutex_unlock(&monitor->dead_mutex);
}

void	*monitor_routine(void *arg)
{
	t_monitor	*monitor;
	int			i;

	monitor = (t_monitor *)arg;
	usleep(1000);
	while (monitor->die == ALIVE)
	{
		i = 0;
		while (i < monitor->p_num && monitor->die == ALIVE)
		{
			if (get_time(monitor->start_time) - monitor->philos[i].last_eat
				> monitor->die_time)
				return (setdead(monitor, i), NULL);
			i++;
		}
		if (is_full(monitor))
			break ;
		usleep(1000);
	}
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_monitor	*monitor;
	t_philo		*philo;

	philo = (t_philo *)arg;
	monitor = philo->monitor;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (philo->die == ALIVE)
	{
		if (dead_check(monitor))
			break ;
		if (feed_philo(monitor, philo))
			break ;
		if (sleep_philo(monitor, philo))
			break ;
		if (think_philo(monitor, philo))
			break ;
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_monitor	monitor;

	if (argc < 5 || argc > 6)
	{
		printf("using: ./philo number_of_philosopher time_to_die time_to_eat \
time_to_sleep number_of_times_each_philosopher_must_eat(optional)\n");
		return (1);
	}
	if (init_monitor(&monitor, argc, argv) == 0)
		return (printf("Invalid arguments\n"), 1);
	monitor.philos = malloc(sizeof(t_philo) * monitor.p_num);
	if (!monitor.philos)
		return (printf("Failed to allocate memory \
			for philosophers\n"), 1);
	if (init_mutex(&monitor) != 0)
	{
		free(monitor.philos);
		printf("Failed to initialize mutexes\n");
		return (1);
	}
	init_philos_threads(&monitor);
	cleanup_mutexes(&monitor, monitor.p_num, 1, 1);
	return (0);
}
