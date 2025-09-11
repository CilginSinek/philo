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

void ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
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
 * 			flags[2] - start_sem, flags[3] - eat_sems
 * 			
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
		sem_close(monitor->start_sem);
		sem_unlink("/start_sem");
	}
	if (flags[3])
	{
		if(monitor->eat_complete != NONE)
		{
			sem_close(monitor->eat_sems);
			sem_unlink("/eat_sems");
		}
	}
}

void cleanup_child(t_monitor *monitor)
{
    sem_close(monitor->forks);
    sem_close(monitor->print_sem);
    sem_close(monitor->start_sem);
    if(monitor->eat_complete != NONE)
        sem_close(monitor->eat_sems);
    if(monitor->philos)
        free(monitor->philos);
}

int	init_semaphores(t_monitor *monitor)
{
    sem_unlink("/forks");
    sem_unlink("/print_sem");
    sem_unlink("/start_sem");
	if(monitor->eat_complete != NONE)
		sem_unlink("/eat_sems");

	monitor->forks = sem_open("/forks", O_CREAT, 0644, monitor->p_num);
	if (monitor->forks == SEM_FAILED)
		return (0);
	monitor->print_sem = sem_open("/print_sem", O_CREAT, 0644, 1);
	if (monitor->print_sem == SEM_FAILED)
	{
		cleanup_semaphores(monitor, (int []){1, 0, 0, 0});
		return (0);
	}
	monitor->start_sem = sem_open("/start_sem", O_CREAT, 0644, 0);
	if (monitor->start_sem == SEM_FAILED)
	{
		cleanup_semaphores(monitor, (int []){1, 1, 0, 0});
		return (0);
	}
	if(monitor->eat_complete != NONE)
	{
		monitor->eat_sems = sem_open("/eat_sems", O_CREAT, 0644, 0);
		if (monitor->eat_sems == SEM_FAILED)
		{
			cleanup_semaphores(monitor, (int []){1, 1, 1, 0});
			return (0);
		}
	}
	return (1);
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
	i = 0;
	while (i < monitor->p_num)
	{
		monitor->philos[i].id = i + 1;
		monitor->philos[i].eat_count = 0;
		monitor->philos[i].last_eat = 0;
		monitor->philos[i].monitor = monitor;
		monitor->philos[i].die = ALIVE;
		monitor->philos[i].forks = 0;
		i++;
	}
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
	ft_putnbr(get_time(monitor->start_time));
	write(1, " ", 1);
	ft_putnbr(philo->id);
	write(1, " ", 1);
	write(1, action, ft_strlen(action));
	write(1, "\n", 1);
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
		return (1);
	}
	return (0);
}

int take_fork(t_philo *philo, t_monitor *monitor)
{
	if (healty_check(philo))
		return (1);
	sem_wait(monitor->forks);
	philo->forks++;
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
	print_action(philo, "is eating");
	philo->last_eat = get_time(monitor->start_time);
	philo->eat_count++;
	if (monitor->eat_complete != NONE && philo->eat_count == monitor->eat_limit)
		sem_post(monitor->eat_sems);
	usleep(monitor->eat_time * 1000);
	sem_post(monitor->forks);
	sem_post(monitor->forks);
	philo->forks = 0;
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
		print_action(philo, "is died");
		philo->die = DIE;
		philo->monitor->die = DIE;
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
		usleep(philo->monitor->eat_time * 500);
	while (philo->die == ALIVE)
	{
		if (healty_check(philo))
			break ;
		if (feed_philo(philo))
			break ;
		if (sleep_philo(philo))
			break ;
		if (think_philo(philo))
			break ;
	}
	if(philo->forks)
	{
		while(philo->forks)
		{
			sem_post(philo->monitor->forks);
			philo->forks--;
		}
	}
	cleanup_child(philo->monitor);
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
}


pid_t eat_watcher(t_monitor *monitor)
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

void	monitoring(t_monitor *monitor)
{
	int	i;
	pid_t eat_pid;

	i = 0;
	eat_pid = -1;
	if(monitor->eat_complete != NONE)
	{
		eat_pid = eat_watcher(monitor);
		// -1 olma durumunu handle et
	}
	start_flag_up(monitor);
	waitpid(-1, NULL, 0);
	i = 0;
	while (i < monitor->p_num)
	{
		kill(monitor->philos[i].pid, SIGKILL);
		waitpid(monitor->philos[i].pid, NULL, 0);
		i++;
	}
	if(eat_pid != -1)
	{
		kill(eat_pid, SIGKILL);
		waitpid(eat_pid, NULL, 0);
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
	if(monitor.philos)
		free(monitor.philos);
	return (0);
}
