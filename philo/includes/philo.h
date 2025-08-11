#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define MALLOC 42
# define MUTEX 43
# define USAGE 1
# define INVALID_ARGS 2
# define THREAD 3

# define FORK "has taken the fork\n"
# define EAT "is eating\n"
# define SLEEP "is sleeping\n"
# define THINK "is thinking\n"
# define DEATH "died\n"
# define DEFORK "has released the fork\n"

typedef struct s_table
{
	int				qty_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				qty_eat;
	long			start_time;
	pthread_mutex_t *forks;
	pthread_mutex_t	write;
}	t_table;

typedef struct s_philo
{
	int				id;
	long			last_eat;
	suseconds_t		qty_eat;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_t		thread;
	t_table			*table;
}	t_philo;

void	ft_error(int err, t_table *table, t_philo *philo);
void	free_forks(t_table *table, int count);

int		ft_init(char **argv, t_table *table, t_philo **philo);
int		ft_atoi(const char *str);
long	ft_get_time(void);

int		ft_run(t_table *table, t_philo *philo);
void	*ft_loop(void *_philo);


#endif