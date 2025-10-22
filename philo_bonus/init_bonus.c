/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:19:23 by iduman            #+#    #+#             */
/*   Updated: 2025/10/22 14:26:26 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_monitor(t_monitor *monitor, int argc, char *argv[])
{
	if (!is_all_numaric(argc, argv))
		return (0);
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
	if (monitor->eat_limit > 0)
		monitor->eat_complete = FALSE;
	else
		monitor->eat_complete = NONE;
	return (1);
}

/**
 * @brief Cleans up semaphores based on which were successfully initialized.
 * @param monitor Pointer to the monitor structure
 * 				containing semaphore references
 * @param flags Array indicating which semaphores were initialized
 * 			(1 if initialized, 0 otherwise)
 * @note flags[0] - forks, flags[1] - print_sem,
 * 			flags[2] - start_sem, flags[3] - eat_sems
 * 			
 */
void	cleanup_semaphores(t_monitor *monitor, int *flags)
{
	if (flags[0])
	{
		sem_close(monitor->forks);
		sem_unlink(monitor->sem_names[0]);
	}
	if (flags[1])
	{
		sem_close(monitor->print_sem);
		sem_unlink(monitor->sem_names[1]);
	}
	if (flags[2])
	{
		sem_close(monitor->start_sem);
		sem_unlink(monitor->sem_names[2]);
	}
	if (flags[3])
	{
		if (monitor->eat_complete != NONE)
		{
			sem_close(monitor->eat_sems);
			sem_unlink(monitor->sem_names[3]);
		}
	}
}

static void	clear_eat_mutex(t_philo *philo)
{
	if (philo->eat_mutex)
	{
		sem_close(philo->eat_mutex);
		sem_unlink(philo->eat_sem_name);
	}
	if (philo->eat_sem_name)
		free(philo->eat_sem_name);
}

void	cleanup_child(t_monitor *monitor, t_philo *philo)
{
	if (philo != NULL)
		clear_eat_mutex(philo);
	free_names(monitor);
	sem_close(monitor->forks);
	sem_close(monitor->print_sem);
	sem_close(monitor->start_sem);
	if (monitor->eat_complete != NONE)
		sem_close(monitor->eat_sems);
	if (monitor->philos)
	{
		free(monitor->philos);
		monitor->philos = NULL;
	}
}

int	init_philos(t_monitor *monitor)
{
	int	i;

	monitor->philos = malloc(sizeof(t_philo) * monitor->p_num);
	if (!monitor->philos)
	{
		printf("Memory allocation failed\n");
		cleanup_semaphores(monitor, (int []){1, 1, 1, 1});
		return (0);
	}
	i = 0;
	while (i < monitor->p_num)
	{
		monitor->philos[i].id = i + 1;
		monitor->philos[i].eat_count = 0;
		monitor->philos[i].last_eat = 0;
		monitor->philos[i].monitor = monitor;
		monitor->philos[i].die = ALIVE;
		monitor->philos[i].forks = 0;
		i++;
	}
	return (1);
}
