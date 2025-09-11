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

static pid_t	eat_watcher(t_monitor *monitor)
{
	pid_t	eat_pid;
	int		eat_count;

	eat_pid = fork();
	if (eat_pid < 0)
	{
		printf("Fork failed\n");
		return (-1);
	}
	if (eat_pid == 0)
	{
		eat_count = 0;
		while (eat_count < monitor->p_num)
		{
			sem_wait(monitor->eat_sems);
			eat_count++;
		}
		cleanup_child(monitor);
		exit(0);
	}
	return (eat_pid);
}

static void	kill_all(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->p_num)
	{
		kill(monitor->philos[i].pid, SIGKILL);
		waitpid(monitor->philos[i].pid, NULL, 0);
		i++;
	}
}

static void	monitoring(t_monitor *monitor)
{
	pid_t	eat_pid;

	if (monitor->eat_complete != NONE)
	{
		eat_pid = eat_watcher(monitor);
		if (eat_pid == -1)
		{
			kill_all(monitor);
			return ;
		}
	}
	start_flag_up(monitor);
	waitpid(-1, NULL, 0);
	kill_all(monitor);
	kill(eat_pid, SIGKILL);
	waitpid(eat_pid, NULL, 0);
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
	if (monitor.philos)
		free(monitor.philos);
	return (0);
}
