/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 21:06:10 by iduman            #+#    #+#             */
/*   Updated: 2025/10/01 14:05:00 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_alive_in_event(t_philo *philo, int event_time)
{
	int long	timeleft;

	timeleft = philo->monitor->die_time - (get_time(philo->monitor->start_time)
			- philo->last_eat);
	if (timeleft < event_time)
	{
		usleep(timeleft * 1000);
		print_events(&philo->monitor->print_mutex, philo->id, "died",
			philo->monitor->start_time);
		pthread_mutex_lock(&philo->monitor->dead_mutex);
		philo->die = DIE;
		philo->monitor->die = DIE;
		pthread_mutex_unlock(&philo->monitor->dead_mutex);
		return (1);
	}
	usleep(event_time * 1000);
	return (0);
}

int	is_all_numaric(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		if (argv[i][j] == '+' || argv[i][j] == '-')
			j++;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int			result;
	int			i;
	signed char	a;

	result = 0;
	i = 0;
	a = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			a = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * a);
}

void	*one_philo_event(void *arg)
{
	struct timeval	start_time;
	t_monitor		*monitor;

	monitor = (t_monitor *)arg;

	gettimeofday(&start_time, NULL);
	print_events(&monitor->print_mutex, 1, "has taken a fork",
		start_time);
	usleep(monitor->die_time * 1000);
	print_events(&monitor->print_mutex, 1, "died", start_time);
	return (NULL);
}
