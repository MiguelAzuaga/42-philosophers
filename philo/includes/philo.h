#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# define MALLOC 42
# define MUTEX 43
# define USAGE 1
# define INVALID_ARGS 2

typedef struct s_data
{
	int				qty_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				qty_eat;
	long			start_time;
	pthread_mutex_t	mutex;
}	t_data;

typedef struct s_philo
{
	int				id;
	long			last_eat;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_data			data;
}	t_philo;

#endif