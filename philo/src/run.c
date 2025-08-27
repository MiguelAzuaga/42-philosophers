/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:14 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/27 17:47:44 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_write_action(t_philo *philo, char *action)
{
	long	timestamp;
	int		end_sim;

	pthread_mutex_lock(&philo->table->lock_state);
	end_sim = philo->table->end_sim;
	timestamp = ft_get_time() - philo->table->start_time;
	pthread_mutex_unlock(&philo->table->lock_state);
	pthread_mutex_lock(&philo->table->write);
	if (!end_sim)
	{
		ft_putnbr(timestamp);
		write(1, " ", 1);
		ft_putnbr(philo->id);
		write(1, " ", 1);
		ft_putstr(action);
	}
	pthread_mutex_unlock(&philo->table->write);
}

int	ft_is_over(t_table *table)
{
	int	res;

	pthread_mutex_lock(&table->lock_state);
	res = table->end_sim;
	pthread_mutex_unlock(&table->lock_state);
	return (res);
}

static int	is_dead(t_philo *philo)
{
	int		dead;
	long	cur;
	long	last_eat;

	dead = 0;
	cur = ft_get_time();
	pthread_mutex_lock(&philo->table->lock_state);
	last_eat = philo->last_eat;
	pthread_mutex_unlock(&philo->table->lock_state);
	if (cur - last_eat > philo->table->time_die)
	{
		ft_write_action(philo, DEAD);
		pthread_mutex_lock(&philo->table->lock_state);
		philo->table->end_sim = 1;
		pthread_mutex_unlock(&philo->table->lock_state);
		dead = 1;
	}
	return (dead);
}

static void	monitor_loop(t_table *table, t_philo *philo)
{
	int	i;
	int	count;

	while (!ft_is_over(table))
	{
		i = 0;
		count = 0;
		while (i < table->qty_philo && !ft_is_over(table))
		{
			if (is_dead(&philo[i]))
				return ;
			pthread_mutex_lock(&table->lock_state);
			if (table->qty_eat > 0 && philo[i].qty_eat >= table->qty_eat)
			{
				count++;
				if (count == table->qty_philo)
					table->end_sim = 1;
			}
			pthread_mutex_unlock(&table->lock_state);
			if (table->end_sim)
				return ;
			i++;
		}
		usleep(500);
	}
}

int	ft_run(t_table *table, t_philo *philo)
{
	int	i;

	if (table->qty_philo == 1)
	{
		ft_write_action(philo, FORK);
		usleep(table->time_die * 1000);
		ft_write_action(philo, DEAD);
		return (0);
	}
	i = -1;
	while (++i < table->qty_philo)
		if (pthread_create(&philo[i].thread, NULL, ft_loop, &philo[i]) != 0)
			return (THREAD);
	monitor_loop(table, philo);
	i = -1;
	while (++i < table->qty_philo)
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (THREAD);
	return (0);
}
