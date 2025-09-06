/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 21:27:09 by iduman            #+#    #+#             */
/*   Updated: 2025/09/04 21:27:09 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

/**
 * @brief Cleans up semaphores based on which were successfully initialized.
 * @param monitor Pointer to the monitor structure
 * 				containing semaphore references
 * @param flags Array indicating which semaphores were initialized
 * 			(1 if initialized, 0 otherwise)
 * @note flags[0] - forks, flags[1] - print_sem,
 * 			flags[2] - dead_sem, flags[3] - start_sem
 */
void	cleanup_semaphores(t_monitor *monitor, int *flags)
{
	if (flags[0])
	{
		sem_close(monitor->forks);
		sem_unlink("/forks");
	}
	if (flags[1])
	{
		sem_close(monitor->print_sem);
		sem_unlink("/print_sem");
	}
	if (flags[2])
	{
		sem_close(monitor->dead_sem);
		sem_unlink("/dead_sem");
	}
	if (flags[3])
	{
		sem_close(monitor->start_sem);
		sem_unlink("/start_sem");
	}
}

int	init_semaphores(t_monitor *monitor)
{
	monitor->forks = sem_open("/forks", O_CREAT, 0644, monitor->p_num);
	if (monitor->forks == SEM_FAILED)
		return (0);
	monitor->print_sem = sem_open("/print_sem", O_CREAT, 0644, 1);
	if (monitor->print_sem == SEM_FAILED)
	{
		cleanup_semaphores(monitor, (int []){1, 0, 0, 0});
		return (0);
	}
	monitor->dead_sem = sem_open("/dead_sem", O_CREAT, 0644, 0);
	if (monitor->dead_sem == SEM_FAILED)
	{
		cleanup_semaphores(monitor, (int []){1, 1, 0, 0});
		return (0);
	}
	monitor->start_sem = sem_open("/start_sem", O_CREAT, 0644, 0);
	if (monitor->start_sem == SEM_FAILED)
	{
		cleanup_semaphores(monitor, (int []){1, 1, 1, 0});
		return (0);
	}
	return (1);
}

char *init_sem_name(int id)
{
	char *name;
	char *id_str;
	char *prefix = "/philo_eat_sem_";

	id_str = ft_itoa(id);
	if (!id_str)
		return (NULL);
	name = malloc(sizeof(char) * (ft_strlen(prefix) + ft_strlen(id_str) + 1));
	if (!name)
		return (free(id_str), NULL);
	while(*prefix)
		*name++ = *prefix++;
	while(*id_str)
		*name++ = *id_str++;
	*name = '\0';
	return (free(id_str), name);
}

void append_sems_monitor(t_monitor *monitor)
{
	int i;

	i = 0;
	while (i < monitor->p_num)
	{
		monitor->eat_sems[i] = monitor->philos[i].philo_eat_sem;
		i++;
	}
}

int	init_philos(t_monitor *monitor)
{
	int	i;
	char *sem_name;

	monitor->philos = malloc(sizeof(t_philo) * monitor->p_num);
	if (!monitor->philos)
	{
		printf("Memory allocation failed\n");
		cleanup_semaphores(monitor, (int []){1, 1, 1, 1});
		return (0);
	}
	monitor->eat_sems = malloc(sizeof(sem_t *) * monitor->p_num);
	if (!monitor->eat_sems)
	{
		printf("Memory allocation failed\n");
		cleanup_semaphores(monitor, (int []){1, 1, 1, 1});
		free(monitor->philos);
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
		sem_name = init_sem_name(i + 1);
		if (!sem_name)
		{
			printf("Semaphore name allocation failed\n");
			cleanup_semaphores(monitor, (int []){1, 1, 1, 1});
			while (--i >= 0)
			{
				sem_close(monitor->philos[i].philo_eat_sem);
				sem_unlink(sem_name);
			}
			free(monitor->philos);
			return (0);
		}
		monitor->philos[i].philo_eat_sem = sem_open(sem_name, O_CREAT, 0644, 0);
		if(monitor->philos[i].philo_eat_sem == SEM_FAILED)
		{
			printf("Semaphore initialization failed\n");
			cleanup_semaphores(monitor, (int []){1, 1, 1, 1});
			while (--i >= 0)
			{
				sem_close(monitor->philos[i].philo_eat_sem);
				sem_unlink(sem_name);
			}
			free(monitor->philos);
			free(sem_name);
			return (0);
		}
		free(sem_name);
		i++;
	}
	append_sems_monitor(monitor);
	return (1);
}

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

void	philosopher_routine(t_philo *philo)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	if (philo->id % 2 == 0)
		usleep(500);
	monitor->philos->last_eat = get_time(monitor->start_time);
	// while (philo->die == ALIVE)
	// {
	// 	//* bşyler yapmamız lazım imdat
	// }
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
			while (1)
				sem_wait(monitor->start_sem);
			philosopher_routine(&monitor->philos[i]);
			exit(0);
		}
		i++;
	}
	start_flag_up(monitor);
}

int everyone_ate(t_monitor *monitor)
{
	int i;

	i = 0;
	while (i < monitor->p_num)
	{
		if (monitor->philos[i].eat_count < monitor->eat_limit)
			return (0);
		i++;
	}
	return (1);
}

void eat_watcher_routine(t_monitor *monitor)
{
	int i;

	char eat_flag;
	eat_flag = 0;
	while (!eat_flag)
	{
		i = 0;
		while (i < monitor->p_num)
		{
			sem_wait(monitor->eat_sems[i]);
			monitor->philos[i].eat_count++;
			if (everyone_ate(monitor))
			{
				eat_flag = 1;
				sem_post(monitor->dead_sem);
				break ;
			}
			i++;
		}
	}
}

void	monitoring(t_monitor *monitor)
{
	int	i;
	int	status;

	if(monitor->eat_limit > 0)
	{
		monitor->eat_watcher = fork();
		if (monitor->eat_watcher < 0)
		{
			sem_post(monitor->dead_sem);
			printf("Fork failed\n");
			return ;
		}
		if (monitor->eat_watcher == 0)
		{
			eat_watcher_routine(monitor);
			exit(0);
		}
	}
	while (1)
		sem_wait(monitor->dead_sem);
	i = 0;
	while (i < monitor->p_num)
	{
		kill(monitor->philos[i].pid, SIGTERM);
		kill(monitor->eat_watcher, SIGTERM);
		i++;
	}
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
	if (!init_monitor(&monitor, argc, argv))
		return (printf("Invalid arguments\n"), 1);
	if (!init_semaphores(&monitor))
		return (printf("Semaphore initialization failed\n"), 1);
	if (!init_philos(&monitor))
		return (printf("Philosopher initialization failed\n"), 1);
	create_start_process(&monitor);
	monitoring(&monitor);
	cleanup_semaphores(&monitor, (int []){1, 1, 1, 1});
	free(monitor.philos);
	return (0);
}
