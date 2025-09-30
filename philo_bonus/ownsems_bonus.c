/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ownsems_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 19:20:34 by iduman            #+#    #+#             */
/*   Updated: 2025/09/30 19:20:34 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

char	*init_name(char *base, int id)
{
	char	*id_str;
	char	*name;
	char	*result;
	int		i;

	id_str = ft_itoa(id);
	if (!id_str)
		return (NULL);
	name = malloc(sizeof(char) * (ft_strlen(base) + ft_strlen(id_str) + 1));
	if (!name)
		return (free(id_str), NULL);
	i = 0;
	result = name;
	while (*base)
		*name++ = *base++;
	while (id_str[i])
		*name++ = id_str[i++];
	*name = '\0';
	free(id_str);
	return (result);
}

void	free_names(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (monitor->sem_names[i])
			free(monitor->sem_names[i]);
		i++;
	}
}

static int	init_sem_names(t_monitor *monitor)
{
	monitor->sem_names[0] = init_name("/forks", get_time_six());
	monitor->sem_names[1] = init_name("/print_sem", get_time_six());
	monitor->sem_names[2] = init_name("/start_sem", get_time_six());
	if (monitor->eat_complete != NONE)
		monitor->sem_names[3] = init_name("/eat_sems", get_time_six());
	else
		monitor->sem_names[3] = NULL;
	if (!monitor->sem_names[0] || !monitor->sem_names[1]
		|| !monitor->sem_names[2]
		|| (monitor->eat_complete != NONE && !monitor->sem_names[3]))
	{
		free_names(monitor);
		return (0);
	}
	return (1);
}

static void	unlink_names(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (monitor->sem_names[i])
			sem_unlink(monitor->sem_names[i]);
		i++;
	}
}

int	init_semaphores(t_monitor *monitor)
{
	char	**names;

	names = monitor->sem_names;
	if (!init_sem_names(monitor))
		return (0);
	unlink_names(monitor);
	monitor->forks = sem_open(names[0], O_CREAT, 0644, monitor->p_num);
	if (monitor->forks == SEM_FAILED)
		return (0);
	monitor->print_sem = sem_open(names[1], O_CREAT, 0644, 1);
	if (monitor->print_sem == SEM_FAILED)
		return (cleanup_semaphores(monitor, (int []){1, 0, 0, 0}), 0);
	monitor->start_sem = sem_open(names[2], O_CREAT | O_EXCL, 0644, 0);
	if (monitor->start_sem == SEM_FAILED)
		return (cleanup_semaphores(monitor, (int []){1, 1, 0, 0}), 0);
	if (monitor->eat_complete != NONE)
	{
		monitor->eat_sems = sem_open(names[3], O_CREAT | O_EXCL, 0644, 0);
		if (monitor->eat_sems == SEM_FAILED)
		{
			cleanup_semaphores(monitor, (int []){1, 1, 1, 0});
			return (0);
		}
	}
	return (1);
}
