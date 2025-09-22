/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:19:15 by iduman            #+#    #+#             */
/*   Updated: 2025/09/22 16:48:08 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long int	get_time(struct timeval start)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec - start.tv_sec) * 1000 + (
			current.tv_usec - start.tv_usec) / 1000);
}

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

static void	ft_putnbr(int n)
{
	long	nbr;

	nbr = n;
	if (nbr < 0)
	{
		nbr *= -1;
		ft_putchar('-');
	}
	if (nbr >= 10)
	{
		ft_putnbr(nbr / 10);
		ft_putchar((nbr % 10) + '0');
		nbr /= 10;
	}
	else
		ft_putchar(nbr + '0');
}

void	print_action(t_philo *philo, char *action)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	sem_wait(monitor->print_sem);
	ft_putnbr(get_time(monitor->start_time));
	write(1, " ", 1);
	ft_putnbr(philo->id);
	write(1, " ", 1);
	write(1,"pid: ",5);
	ft_putnbr(getpid());
	write(1, " ", 1);
	write(1, action, ft_strlen(action));
	write(1, "\n", 1);
	sem_post(monitor->print_sem);
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
