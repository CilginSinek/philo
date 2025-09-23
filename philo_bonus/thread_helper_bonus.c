/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_helper_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:26:43 by iduman            #+#    #+#             */
/*   Updated: 2025/09/23 17:58:14 by iduman           ###   ########.fr       */
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
			return (NULL);
		}
	}
	return (NULL);
}
