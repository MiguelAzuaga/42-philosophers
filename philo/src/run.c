/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:14 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/14 09:59:56 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_action(t_philo *philo, char *action)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->write);
	timestamp = ft_get_time() - philo->table->start_time;
	if (!philo->table->end_sim)
		printf("%-5ld %2d %s", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->table->write);
}

int	is_dead(t_philo *philo)
{
	int		dead;
	long	cur;

	dead = 0;
	cur = ft_get_time();
	pthread_mutex_lock(&philo->table->lock_state);
	if (cur - philo->last_eat >= philo->table->time_die
		&& !philo->table->end_sim)
	{
		philo->table->end_sim = 1;
		pthread_mutex_lock(&philo->table->write);
		printf("%-5ld %2d %s", cur - philo->table->start_time, philo->id, DEAD);
		pthread_mutex_unlock(&philo->table->write);
		dead = 1;
	}
	pthread_mutex_unlock(&philo->table->lock_state);
	return (dead);
}

int	ft_run(t_table *table, t_philo *philo)
{
	int	i;

	if (table->qty_philo == 1)
	{
		write_action(philo, FORK);
		usleep(table->time_die);
		write_action(philo, DEAD);
		ft_error(-1, table, philo);
	}
	i = 0;
	while (i < table->qty_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, ft_loop, &philo[i]) != 0)
			return (THREAD);
		i++;
	}
	i = 0;
	while (i < table->qty_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (THREAD);
		i++;
	}
	return (0);
}
