/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_helper_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:33:46 by iduman            #+#    #+#             */
/*   Updated: 2025/10/01 13:53:31 by iduman           ###   ########.fr       */
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
<<<<<<< HEAD
	if (philo->forks == 1 && monitor->p_num == 1)
		return (1);
=======
	if (monitor->p_num == 1 && philo->forks == 1)
		return (is_alive_in_event(philo, monitor->die_time), 1);
>>>>>>> 2e1e0a0693e9268d18a3636c02451e50976bf610
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
<<<<<<< HEAD
=======
		print_action(philo, "died");
		philo->die = DIE;
		philo->monitor->die = DIE;
>>>>>>> 2e1e0a0693e9268d18a3636c02451e50976bf610
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
