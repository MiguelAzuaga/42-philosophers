/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:14 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/20 15:58:01 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_action(t_philo *philo, char *action)
{
	long	timestamp;
	int		end_sim;

	pthread_mutex_lock(&philo->table->lock_state);
	end_sim = philo->table->end_sim;
	timestamp = ft_get_time() - philo->table->start_time;
	pthread_mutex_unlock(&philo->table->lock_state);
	pthread_mutex_lock(&philo->table->write);
	if (!end_sim)
		printf("%-5ld %3d %s", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->table->write);
}

int	is_over(t_table *table)
{
	int	res;

	pthread_mutex_lock(&table->lock_state);
	res = table->end_sim;
	pthread_mutex_unlock(&table->lock_state);
	return (res);
}

int	is_dead(t_philo *philo)
{
	int		dead;
	long	cur;
	long	last_eat;
	long	time_die;

	dead = 0;
	cur = ft_get_time();
	pthread_mutex_lock(&philo->table->lock_state);
	last_eat = philo->last_eat;
	time_die = philo->table->time_die;
	pthread_mutex_unlock(&philo->table->lock_state);
	if (cur - last_eat > time_die)
		dead = 1;
	return (dead);
}

void monitor_loop(t_table *table, t_philo *philo)
{
	int	i;
	int	count;

	while (!is_over(table))
	{
		i = 0;
		count = 0;
		while (i < table->qty_philo && !is_over(table))
		{
			if (is_dead(&philo[i]))
			{
				pthread_mutex_lock(&table->lock_state);
				if (!table->end_sim)
				{
					table->end_sim = 1;
					pthread_mutex_unlock(&table->lock_state);
					pthread_mutex_lock(&philo->table->write);
					printf("%-5ld %3d %s", ft_get_time() - table->start_time, philo->id, DEAD);
					pthread_mutex_unlock(&philo->table->write);
				}
				else
					pthread_mutex_unlock(&table->lock_state);
				return ;
			}
			pthread_mutex_lock(&table->lock_state);
			if (table->qty_eat > 0 && philo[i].qty_eat >= table->qty_eat)
			{
				count++;
				if (count == table->qty_philo)
				{
					table->end_sim = 1;
					pthread_mutex_unlock(&table->lock_state);
					pthread_mutex_lock(&philo->table->write);
					printf("%-5ld %s", ft_get_time() - table->start_time, FINISH);
					pthread_mutex_unlock(&philo->table->write);
					return ;
				}
			}
			pthread_mutex_unlock(&table->lock_state);
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
		write_action(philo, FORK);
		usleep(table->time_die * 1000);
		write_action(philo, DEAD);
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
