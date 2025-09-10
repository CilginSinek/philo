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

long int	get_time(struct timeval start)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec - start.tv_sec) * 1000 + (
			current.tv_usec - start.tv_usec) / 1000);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
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

static int	counter(int n)
{
	int		c;

	c = 0;
	if (n < 0)
		c++;
	while (n)
	{
		n /= 10;
		c++;
	}
	return (c);
}
char	*ft_strdup(const char *str)
{
	char	*nstr;
	size_t	i;

	nstr = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!nstr)
		return ((char *) NULL);
	i = 0;
	while (str[i] != '\0')
	{
		nstr[i] = str[i];
		i++;
	}
	nstr[i] = '\0';
	return (nstr);
}

char	*ft_itoa(int n)
{
	char			*str;
	int				len;
	unsigned int	num;

	if (n == 0)
		return (ft_strdup("0"));
	len = counter(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n < 0)
		num = -n;
	else
		num = n;
	while (num)
	{
		str[--len] = (num % 10) + '0';
		num /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
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
	if(monitor->eat_limit > 0)
		monitor->eat_complete = FALSE;
	else
		monitor->eat_complete = NONE;
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

    sem_unlink("/forks");
    sem_unlink("/print_sem");
    sem_unlink("/dead_sem");
    sem_unlink("/start_sem");

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
	char *prefix;
	int i;
	int j;

	i = 0;
	j = 0;
	prefix = "philo_eat_sem_";
	id_str = ft_itoa(id);
	if (!id_str)
		return (NULL);
	name = malloc(sizeof(char) * (ft_strlen(prefix) + ft_strlen(id_str) + 1));
	if (!name)
		return (free(id_str), NULL);
	while(*prefix)
		name[i++] = *prefix++;
	while(id_str[j])
		name[i++] = id_str[j++];
	name[i] = '\0';
	return (free(id_str), name);
}

int init_eat_sems(t_monitor *monitor)
{
	int i;

	monitor->e_sem_names = malloc(sizeof(char *) * monitor->p_num);
	if (!monitor->e_sem_names)
		return (0);
	i = 0;
	while (i < monitor->p_num)
	{
		monitor->e_sem_names[i] = init_sem_name(i + 1);
		if (!monitor->e_sem_names[i])
		{
			while (--i >= 0)
				free(monitor->e_sem_names[i]);
			free(monitor->e_sem_names);
			return (0);
		}
		i++;
	}
	return (1);
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

void clean_eat_sems(t_monitor *monitor)
{
	int i;
	char *sem_name;

	i = 0;
	while (i < monitor->p_num)
	{
		sem_name = monitor->e_sem_names[i];
		sem_close(monitor->philos[i].philo_eat_sem);
		sem_unlink(sem_name);
		free(sem_name);
		i++;
	}
	free(monitor->philos);
	free(monitor->eat_sems);
}

int	init_philos(t_monitor *monitor)
{
	int	i;

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
	if (!init_eat_sems(monitor))
	{
		printf("Memory allocation failed\n");
		cleanup_semaphores(monitor, (int []){1, 1, 1, 1});
		free(monitor->philos);
		free(monitor->eat_sems);
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
		monitor->philos[i].philo_eat_sem = sem_open(monitor->e_sem_names[i], O_CREAT, 0644, 0);
		if(monitor->philos[i].philo_eat_sem == SEM_FAILED)
		{
			printf("Semaphore initialization failed\n");
			cleanup_semaphores(monitor, (int []){1, 1, 1, 1});
			clean_eat_sems(monitor);
			return (0);
		}
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

void print_action(t_philo *philo, char *action)
{
	t_monitor *monitor;

	monitor = philo->monitor;
	sem_wait(monitor->print_sem);
	printf("%ld %d %s\n", get_time(monitor->start_time), philo->id, action);
	sem_post(monitor->print_sem);
}

int healty_check(t_philo *philo)
{
	t_monitor *monitor;

	monitor = philo->monitor;
	if (get_time(monitor->start_time) - philo->last_eat > monitor->die_time)
	{
		philo->die = DIE;
		monitor->die = DIE;
		print_action(philo, "is died");
		sem_post(monitor->dead_sem);
		return (1);
	}
	return (0);
}

int take_fork(t_philo *philo, t_monitor *monitor)
{
	if (healty_check(philo))
		return (1);
	sem_wait(monitor->forks);
	print_action(philo, "has taken a fork");
	return (0);
}

int take_forks(t_philo *philo)
{
	t_monitor *monitor;

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

int	feed_philo(t_philo *philo)
{
	t_monitor *monitor;

	monitor = philo->monitor;
	if (take_forks(philo))
		return (1);
	philo->last_eat = get_time(monitor->start_time);
	philo->eat_count++;
	print_action(philo, "is eating");
	sem_post(philo->philo_eat_sem);
	usleep(monitor->eat_time * 1000);
	sem_post(monitor->forks);
	sem_post(monitor->forks);
	return (0);
}

int is_alive_in_event(t_philo *philo, int event_time)
{
	int long timeleft;

	timeleft = philo->monitor->die_time - (get_time(philo->monitor->start_time)
			- philo->last_eat);
	if (timeleft < event_time)
	{
		usleep(timeleft * 1000);
		return (1);
	}
	usleep(event_time * 1000);
	return (0);
}

int sleep_philo(t_philo *philo)
{
	t_monitor *monitor;

	monitor = philo->monitor;
	if (healty_check(philo))
		return (1);
	print_action(philo, "is sleeping");
	if (is_alive_in_event(philo, monitor->sleep_time))
		return (1);
	return (0);
}

int think_philo(t_philo *philo)
{
	if (healty_check(philo))
		return (1);
	print_action(philo, "is thinking");
	return (0);
}

void	philosopher_routine(t_philo *philo)
{
	philo->last_eat = get_time(philo->monitor->start_time);
	if (philo->id % 2 == 0)
		usleep(500);
	while (philo->die == ALIVE)
	{
		if (healty_check(philo))
		{
			printf("%d healty_check break\n", philo->id);
			break ;
		}
		if (feed_philo(philo))
		{
			printf("%d feed_philo break\n", philo->id);
			break ;
		}
		if (sleep_philo(philo))
		{
			printf("%d sleep_philo break\n", philo->id);
			break ;
		}
		if (think_philo(philo))
		{
			printf("%d think_philo break\n", philo->id);
			break ;
		}
	}
	exit(0);
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

void eat_watcher(t_monitor *monitor)
{
	int i;
	int eat_count;

	i = 0;
	while(i < monitor->p_num)
	{
		monitor->philos[i].eat_pid = fork();
		if(monitor->philos[i].eat_pid < 0)
		{
			while(--i >= 0)
				kill(monitor->philos[i].eat_pid, SIGTERM);
			printf("Fork failed\n");
			return ;
		}
		if(monitor->philos[i].eat_pid == 0)
		{
			eat_count = 0;
			while(eat_count < monitor->eat_limit)
			{
				sem_wait(monitor->philos[i].philo_eat_sem);
				eat_count++;
			}
			printf("eat_pidler exit atıo\n");
			exit(0);
		}
		i++;
	}
}

void	monitoring(t_monitor *monitor)
{
	int	i;

	i = 0;
	if(monitor->eat_complete != NONE)
	{
		eat_watcher(monitor);
		monitor->eat_complete = TRUE;
	}
	waitpid(-1, NULL, 0);
	i = 0;
	while (i < monitor->p_num)
	{
		printf("calisma aq\n");
		kill(monitor->philos[i].pid, SIGTERM);
		if(monitor->eat_complete == FALSE)
			kill(monitor->philos[i].eat_pid, SIGTERM);
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
	free(monitor.eat_sems);
	return (0);
}
