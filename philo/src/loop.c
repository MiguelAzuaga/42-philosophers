/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:07 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/21 06:10:16 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	eats(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->table->lock_state);
	philo->last_eat = ft_get_time();
	pthread_mutex_unlock(&philo->table->lock_state);
	write_action(philo, EAT);
	usleep(philo->table->time_eat * 1000);
	put_forks(philo);
	pthread_mutex_lock(&philo->table->lock_state);
	if (philo->table->qty_eat > 0)
		philo->qty_eat++;
	pthread_mutex_unlock(&philo->table->lock_state);
}

void	*ft_loop(void *_philo)
{
	t_philo	*philo;

	philo = (t_philo *)_philo;
	if (!(philo->id % 2))
		usleep(1000);
	while (!is_over(philo->table))
	{
		eats(philo);
		if (is_over(philo->table))
			break ;
		write_action(philo, SLEEP);
		usleep(philo->table->time_sleep * 1000);
		write_action(philo, THINK);
		if (philo->table->qty_philo % 2 && philo->id % 2)
			usleep(1000);
	}
	return (NULL);
}
