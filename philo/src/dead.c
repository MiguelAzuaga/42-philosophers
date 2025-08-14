/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:03 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/14 15:46:21 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	erro_msg(int error)
{
	if (error == MALLOC)
		write(2, "Malloc error\n", 13);
	else if (error == USAGE)
		write(2, "Usage:./philo <number_of_philosophers> <time_to_die> <time_\
to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n", 127);
	else if (error == INVALID_ARGS)
		write(2, "Invalid arguments\n", 18);
	else if (error == MUTEX)
		write(2, "Mutex error\n", 12);
	else if (error == THREAD)
		write(2, "Thread error\n", 13);
	return ;
}

void	free_forks(t_table *table, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
	table->forks = NULL;
	return ;
}

int	ft_error(int err, t_table *table, t_philo *philo)
{
	if (err != 0 && err != -42)
	{
		if (table)
		{
			pthread_mutex_destroy(&table->write);
			pthread_mutex_destroy(&table->lock_state);
			if (table && table->forks)
				free_forks(table, table->qty_philo);
		}
		if (philo)
			free(philo);
	}
	if (err <= 0)
		return (0);
	erro_msg(err);
	return (1);
}
