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

long int get_time(struct timeval start)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec - start.tv_sec) * 1000 + (current.tv_usec - start.tv_usec) / 1000);
}

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
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
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
		i++;
	}
	if (pthread_mutex_init(&monitor->print_mutex, NULL) != 0)
	{
		while (--i >= 0)
			pthread_mutex_destroy(&monitor->philos[i].left_fork);
		return (1);
	}
	return (0);
}

//check them all
int healthy_check(t_monitor *monitor, t_philo *philo)
{
	long int current;

	current = get_time(monitor->start_time);
	printf("debug: isdead: %d last eat: %ld id: %d current: %ld \n",current - philo->last_eat > monitor->die_time, philo->last_eat, philo->id ,current );
	if (current - philo->last_eat > monitor->die_time)
	{
		monitor->die = DIE;
		pthread_mutex_lock(&monitor->print_mutex);
		printf("%ld %d died\n", current, philo->id);
		pthread_mutex_unlock(&monitor->print_mutex);
		return (1);
	}
	return (0);
}

void kill_philo(t_monitor *monitor, t_philo *philo)
{
	long int current;

	current = get_time(monitor->start_time);
	monitor->die = DIE;
	printf("%ld %d died\n", current, philo->id);
}

int is_full(t_monitor *monitor)
{
	int	i;

	i = 0;
	if (monitor->eat_limit == -1)
		return (0);
	while (i < monitor->p_num)
	{
		if (monitor->philos[i].eat_count < monitor->eat_limit)
			return (0);
		i++;
	}
	monitor->die = DIE;
	return (1);
}

int is_alive_in_event(long int event_time, t_monitor *monitor, t_philo *philo)
{
	long int a;

	a = get_time(monitor->start_time) - philo->last_eat + event_time - monitor->die_time;
	if (a > 0)
	{
		usleep(a * 1000);
		kill_philo(monitor, philo);
		return (1);
	}
	usleep(event_time * 1000);
	return (0);
}

int feed_philo(t_monitor *monitor, t_philo *philo)
{
	if(healthy_check(monitor, philo))
		return(1);
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&monitor->print_mutex);
	printf("%ld %d is eating\n", get_time(monitor->start_time), philo->id);
	pthread_mutex_unlock(&monitor->print_mutex);
	usleep(monitor->eat_time * 1000);
	philo->eat_count++;
	philo->last_eat = get_time(monitor->start_time);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
	return (0);
}

int sleep_philo(t_monitor *monitor, t_philo *philo)
{
	if(healthy_check(monitor, philo))
		return(1);
	pthread_mutex_lock(&monitor->print_mutex);
	printf("%ld %d is sleeping\n", get_time(monitor->start_time), philo->id);
	pthread_mutex_unlock(&monitor->print_mutex);
	if (is_alive_in_event(monitor->sleep_time, monitor, philo) == 1)
	{
		printf("%i uyurken öldü\n",philo->id);
		return (1);
	}

	return (0);
}

int think_philo(t_monitor *monitor, t_philo *philo)
{
	if(healthy_check(monitor, philo))
		return (1);
	pthread_mutex_lock(&monitor->print_mutex);
	printf("%ld %d is thinking\n", get_time(monitor->start_time), philo->id);
	pthread_mutex_unlock(&monitor->print_mutex);
	return (0);
}

void *philo_routine(void *arg)
{
	t_monitor	*monitor;
	t_philo		*philo;

	philo = (t_philo *)arg;
	monitor = philo->monitor;
	if (philo->id % 2 == 0)
		usleep(500);
	while (monitor->die == ALIVE)
	{
		if (healthy_check(monitor, philo) || is_full(monitor))
			break ;
		if (feed_philo(monitor, philo))
			break ;
		if (sleep_philo(monitor, philo))
		{
			printf("ölseneoe\n");
			break ;
		}

		if (think_philo(monitor, philo))
			break ;
	}
	return (NULL);
}

void init_philos(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->p_num)
	{
		monitor->philos[i].id = i + 1;
		monitor->philos[i].eat_count = 0;
		monitor->philos[i].last_eat = 0;
		monitor->philos[i].monitor = monitor;
		i++;
	}
}

void	init_philos_threads(t_monitor *monitor)
{
	int	i;
	int	c;

	c = 0;
	gettimeofday(&monitor->start_time, NULL);
	init_philos(monitor);
	while (c < monitor->p_num)
	{
		if (pthread_create(&monitor->philos[c].thread,
				NULL, philo_routine, &monitor->philos[c]) != 0)
		{
			monitor->die = DIE;
			break ;
		}
		c++;
	}
	i = 0;
	if (monitor->die == DIE)
		printf("Failed to create threads\n");
	while (i < c)
	{
		pthread_join(monitor->philos[i].thread, NULL);
		i++;
	}
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
	clean_up(&monitor);
	return (0);
}
