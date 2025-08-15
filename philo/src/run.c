/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:14 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/15 16:32:19 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_action(t_philo *philo, char *action)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->write);
	timestamp = ft_get_time() - philo->table->start_time;
	if (!philo->table->end_sim)
		printf("%-5ld %3d %s", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->table->write);
}

void	sleep_duration(t_philo *philo, long action)
{
	long cur;

	cur = ft_get_time();

	pthread_mutex_lock(&philo->table->lock_state);
	if (philo->last_eat + philo->table->time_die < cur + action && !philo->table->end_sim)
	{
		usleep(((philo->last_eat + philo->table->time_die) - cur) * 1000);
		write_action(philo, DEAD);
		philo->table->end_sim = 1;
	}
	else
		usleep(action * 1000);
	pthread_mutex_unlock(&philo->table->lock_state);
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
		dead = 1;
	}
	if (dead)
	{
		write_action(philo, DEAD);
		philo->table->end_sim = 1;
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
		usleep(table->time_die * 1000);
		pthread_mutex_lock(&philo->table->write);
		printf("%-5ld %3d %s", ft_get_time() - philo->table->start_time,
			philo->id, DEAD);
		pthread_mutex_unlock(&philo->table->write);
		return (0);
	}
	i = -1;
	while (++i < table->qty_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, ft_loop, &philo[i]) != 0)
			return (THREAD);
	}
	i = -1;
	while (++i < table->qty_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (THREAD);
	}
	return (0);
}
