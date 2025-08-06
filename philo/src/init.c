#include "philo.h"

int	init_philo(t_table *table, t_philo **philo)
{
	int	i;

	*philo = malloc(table->qty_philo * sizeof(t_philo));
	if (!*philo)
		ft_error(MALLOC, table, NULL);
	i = 0;
	while (i < table->qty_philo)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].last_eat = ft_get_time();
		(*philo)[i].l_fork = &table->forks[i];
		(*philo)[i].r_fork = &table->forks[(i + 1) % table->qty_philo];
		(*philo)[i].table = table;
		i++;
	}
	return (0);
}

int	init_forks(t_table *table)
{
	int	i;

	table->forks = malloc(table->qty_philo * sizeof(pthread_mutex_t));
	if (!table->forks)
		ft_error(MALLOC, table, NULL);
	i = 0;
	while (i < table->qty_philo)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			free_forks(table, i);
			return (MUTEX);
		}
		i++;
	}
	return (0);
}

int	init_args(char **argv, t_table *table)
{
	table->qty_philo = ft_atoi(argv[1]);
	table->time_die = ft_atoi(argv[2]);
	table->time_eat = ft_atoi(argv[3]);
	table->time_sleep = ft_atoi(argv[4]);
	table->qty_eat = -1;
	table->start_time = ft_get_time();
	if (argv[5])
		table->qty_eat = ft_atoi(argv[5]);
	if (table->qty_philo == 0 || table->qty_eat == 0)
		return (INVALID_ARGS);
	if (pthread_mutex_init(&table->write, NULL) != 0)
		return (MUTEX);
	return (0);
}

int	ft_init(char **argv, t_table *table, t_philo **philo)
{
	ft_error(init_args(argv, table), table, NULL);
	ft_error(init_forks(table), table, NULL);
	ft_error(init_philo(table, philo), table, *philo);
	return (0);
}
