#include "philo.h"

void	write_action(t_philo *philo, char *action)
{
	long timestamp;

	timestamp = ft_get_time() - philo->table->start_time;
	printf("%5ld %2d %s", timestamp, philo->id, action);
}

void	thinks(t_philo *philo)
{
	(void) philo;
}

void	sleeps(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->write);
	write_action(philo, SLEEP);
	usleep(philo->table->time_sleep);
	pthread_mutex_unlock(&philo->table->write);
}

void	eats(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->write);
	write_action(philo, EAT);
	usleep(philo->table->time_eat);
	pthread_mutex_unlock(&philo->table->write);
}

void	*ft_loop(void *_philo)
{
	t_philo *philo;

	philo = (t_philo *)_philo;
	while (1)
	{
		eats(philo);
		sleeps(philo);
		thinks(philo);
	}
	return (NULL);
}