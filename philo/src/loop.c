/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:07 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/19 18:12:32 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	put_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->l_fork);
		write_action(philo, DEFORK);
		pthread_mutex_unlock(philo->r_fork);
		write_action(philo, DEFORK);

	}
	else
	{
		pthread_mutex_unlock(philo->r_fork);
		write_action(philo, DEFORK);
		pthread_mutex_unlock(philo->l_fork);
		write_action(philo, DEFORK);
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
	philo->last_eat = ft_get_time() + philo->table->time_eat;
	write_action(philo, EAT);
	usleep(philo->table->time_eat * 1000);
	write_action(philo, FEAT);
	put_forks(philo);
	if (philo->table->qty_eat > 0)
		philo->qty_eat++;
}

void	*ft_loop(void *_philo)
{
	t_philo	*philo;

	philo = (t_philo *)_philo;
	if (philo->id % 2)
		usleep(1000);
	while (!philo->table->end_sim)
	{
		eats(philo);
		if (philo->table->end_sim)
			break ;
		write_action(philo, SLEEP);
		usleep(philo->table->time_sleep * 1000);
		write_action(philo, THINK);
	}
	return (NULL);
}
