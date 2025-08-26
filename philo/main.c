#include "philo.h"

void init_monitor(t_monitor *monitor,int argc, char *argv[])
{
	monitor->p_num = ft_atoi(argv[1]);
	monitor->die_time = ft_atoi(argv[2]);
	monitor->eat_time = ft_atoi(argv[3]);
	monitor->sleep_time = ft_atoi(argv[4]);
	if(argc == 6)
		monitor->eat_limit = ft_atoi(argv[5]);
	else
		monitor->eat_limit = -1;
}

int main (int argc, char *argv[])
{
	t_monitor monitor;

	//* input handle
	if(argc != 5 || argc != 6)
	{
		printf("using: ./philo number_of_philosopher time_to_die time_to_eat \
        time_to_sleep number_of_times_each_philosopher_must_eat(optional)\n");
		return (1);
    }
	//* init monitor
	init_monitor(&monitor, argc, argv);
	

}