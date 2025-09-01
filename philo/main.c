/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:19:11 by iduman            #+#    #+#             */
/*   Updated: 2025/09/01 20:19:11 by iduman           ###   ########.fr       */
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
	if (argc == 6 || ft_atoi(argv[5]) <= 0)
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
		{
			while (--i >= 0)
				pthread_mutex_destroy(&monitor->philos[i].left_fork);
			return (1);
		}
		monitor->philos[i].right_fork = &monitor->philos[
			(i + 1) % monitor->p_num].left_fork;
	}
	if (pthread_mutex_init(&monitor->print_mutex, NULL) != 0)
	{
		while (--i >= 0)
			pthread_mutex_destroy(&monitor->philos[i].left_fork);
		return (1);
	}
	return (0);
}

void	init_philos_threads(t_monitor *monitor)
{
	int	i;
	int	created;

	created = 0;
	while (created < monitor->p_num)
	{
		if (pthread_create(&monitor->philos[created++].thread,
				NULL, philo_routine, &monitor->philos[created]) != 0)
		{
			monitor->die = DIE;
			break ;
		}
		created++;
	}
	i = 0;
	if (monitor->die == DIE)
		printf("Failed to create threads\n");
	while (i < created)
		pthread_join(monitor->philos[i++].thread, NULL);
}

void	clean_up(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->p_num)
	{
		pthread_mutex_destroy(&monitor->philos[i].left_fork);
		i++;
	}
	pthread_mutex_destroy(&monitor->print_mutex);
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
	{
		printf("Invalid arguments\n");
		return (1);
	}
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
	return (0);
}
