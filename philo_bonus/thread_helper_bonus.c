/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_helper_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:26:43 by iduman            #+#    #+#             */
/*   Updated: 2025/09/22 17:59:38 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*dead_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->die == ALIVE)
	{
		usleep(100);
		if (get_time(philo->monitor->start_time
			) - philo->last_eat > philo->monitor->die_time)
		{
			philo->die = DIE;
			philo->monitor->die = DIE;
			print_action(philo, "is died");
			if (philo->forks)
			{
				while (philo->forks)
				{
					sem_post(philo->monitor->forks);
					philo->forks--;
				}
			}
			cleanup_child(philo->monitor);
			exit(0);
		}
	}
	return (NULL);
}
