/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:22 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/27 17:45:12 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

# define MALLOC 42
# define MUTEX 43
# define USAGE 1
# define INVALID_ARGS 2
# define THREAD 3

# define DEAD "\033[91mdied\033[0m\n"
# define THINK "is thinking\n"
# define EAT "\033[92mis eating\033[0m\n"
# define SLEEP "is sleeping\n"
# define FORK "has taken a fork\n"

typedef struct s_table
{
	int				qty_philo;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	int				qty_eat;
	long			start_time;
	int				end_sim;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write;
	pthread_mutex_t	lock_state;
}	t_table;

typedef struct s_philo
{
	int				id;
	int				qty_eat;
	long			last_eat;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_t		thread;
	t_table			*table;
}	t_philo;

int		ft_error(int err, t_table *table, t_philo *philo);
void	ft_free_forks(t_table *table, int count);
int		ft_is_over(t_table *table);

int		ft_init(char **argv, t_table *table, t_philo **philo);
int		ft_atoi(const char *str);
long	ft_get_time(void);
void	ft_putnbr(long n);
void	ft_putstr(const char *s);

int		ft_run(t_table *table, t_philo *philo);
void	*ft_loop(void *_philo);
void	ft_write_action(t_philo *philo, char *action);

#endif