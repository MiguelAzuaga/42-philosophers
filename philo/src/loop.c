/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:07 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/15 16:31:49 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_mutex_lock(&philo->table->lock_state);
	philo->last_eat = ft_get_time();
	philo->table->total_eat++;
	pthread_mutex_unlock(&philo->table->lock_state);
	write_action(philo, EAT);
	sleep_duration(philo, philo->table->time_eat);
	put_forks(philo);
	if (philo->table->qty_eat > 0)
	{
		philo->qty_eat++;
		if (philo->qty_eat == philo->table->qty_eat)
		{
			pthread_mutex_lock(&philo->table->lock_state);
			philo->table->philo_finished++;
			if (philo->table->philo_finished == philo->table->qty_philo)
				philo->table->end_sim = 3;
			pthread_mutex_unlock(&philo->table->lock_state);
		}
	}
}

static int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->table->lock_state);
	stop = philo->table->end_sim || (philo->table->qty_eat > 0
			&& philo->qty_eat >= philo->table->qty_eat);
	pthread_mutex_unlock(&philo->table->lock_state);
	return (stop);
}

void	*ft_loop(void *_philo)
{
	t_philo	*philo;

	philo = (t_philo *)_philo;
	while (!is_dead(philo) && !should_stop(philo))
	{
		eats(philo);
		if (is_dead(philo) || should_stop(philo))
			break ;
		write_action(philo, SLEEP);
		sleep_duration(philo, philo->table->time_sleep);
		write_action(philo, THINK);
	}
	return (NULL);
}
