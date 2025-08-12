/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:14 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/12 06:20:45 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_philo *philo)
{
	if (ft_get_time() - philo->last_eat >= philo->table->time_die)
	{
		philo->table->end_sim = 1;
		pthread_mutex_lock(&philo->table->write);
		printf("%ld %d %s", ft_get_time() - philo->table->start_time,
			philo->id, DEATH);
		pthread_mutex_unlock(&philo->table->write);
		return (1);
	}
	return (0);
}

int	ft_run(t_table *table, t_philo *philo)
{
	int	i;

	if (table->qty_philo == 1)
	{
		write_action(philo, FORK);
		usleep(table->time_die);
		write_action(philo, DEATH);
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
