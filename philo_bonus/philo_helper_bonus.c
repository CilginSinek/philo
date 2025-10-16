/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_helper_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:33:46 by iduman            #+#    #+#             */
/*   Updated: 2025/09/11 17:33:46 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	start_flag_up(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->p_num)
	{
		sem_post(monitor->start_sem);
		i++;
	}
}

static int	take_fork(t_philo *philo, t_monitor *monitor)
{
	if (healty_check(philo))
		return (1);
	if (philo->forks == 1 && monitor->p_num == 1)
		return (1);
	sem_wait(monitor->forks);
	if (healty_check(philo))
	{
		sem_post(monitor->forks);
		return (1);
	}
	philo->forks++;
	print_action(philo, "has taken a fork");
	return (0);
}

int	take_forks(t_philo *philo)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	if (take_fork(philo, monitor))
		return (1);
	if (take_fork(philo, monitor))
	{
		sem_post(monitor->forks);
		return (1);
	}
	return (0);
}

int	is_alive_in_event(t_philo *philo, int event_time)
{
	int long	timeleft;

	sem_wait(philo->eat_mutex);
	timeleft = philo->monitor->die_time - (get_time(philo->monitor->start_time)
			- philo->last_eat);
	sem_post(philo->eat_mutex);
	if (timeleft < event_time)
	{
		usleep(timeleft * 1000);
		return (1);
	}
	usleep(event_time * 1000);
	return (0);
}

void	create_start_process(t_monitor *monitor)
{
	int	i;

	i = 0;
	gettimeofday(&monitor->start_time, NULL);
	while (i < monitor->p_num)
	{
		monitor->philos[i].pid = fork();
		monitor->philos[i].last_eat = get_time(monitor->start_time);
		if (monitor->philos[i].pid < 0)
		{
			while (--i >= 0)
				kill(monitor->philos[i].pid, SIGTERM);
			printf("Fork failed\n");
			return ;
		}
		if (monitor->philos[i].pid == 0)
		{
			sem_wait(monitor->start_sem);
			philosopher_routine(&monitor->philos[i]);
		}
		i++;
	}
}
