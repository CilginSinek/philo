/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_helper_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:26:43 by iduman            #+#    #+#             */
/*   Updated: 2025/09/29 21:44:20 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*dead_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->die == ALIVE)
	{
		usleep(1000);
		sem_wait(philo->eat_mutex);
		if (get_time(philo->monitor->start_time
			) - philo->last_eat > philo->monitor->die_time)
		{
			philo->die = DIE;
			philo->monitor->die = DIE;
			print_action(philo, "is died");
			sem_post(philo->eat_mutex);
			return (NULL);
		}
		sem_post(philo->eat_mutex);
	}
	return (NULL);
}
