/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:07 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/12 06:58:55 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_action(t_philo *philo, char *action)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->write);
	timestamp = ft_get_time() - philo->table->start_time;
	if (!philo->table->end_sim)
		printf("%ld %d %s", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->table->write);
}

void	put_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		write_action(philo, FORK);
		pthread_mutex_lock(philo->r_fork);
		write_action(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		write_action(philo, FORK);
		pthread_mutex_lock(philo->l_fork);
		write_action(philo, FORK);
	}
}

void	eats(t_philo *philo)
{
	take_forks(philo);
	philo->last_eat = ft_get_time();
	write_action(philo, EAT);
	usleep(philo->table->time_eat);
	put_forks(philo);
	if (philo->table->qty_eat > 0)
	{
		philo->qty_eat++;
		if (philo->qty_eat == philo->table->qty_eat)
		{
			pthread_mutex_lock(&philo->table->finish);
			philo->table->philo_finished++;
			if (philo->table->philo_finished == philo->table->qty_philo)
				philo->table->end_sim = 1;
			pthread_mutex_unlock(&philo->table->finish);
		}
	}
}

void	*ft_loop(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	while (!is_dead(philo) && !philo->table->end_sim)
	{
		if (!philo->table->end_sim)
			eats(philo);
		if (!philo->table->end_sim)
		{
			write_action(philo, SLEEP);
			usleep(philo->table->time_sleep);
		}
		write_action(philo, THINK);
	}
	return (NULL);
}
