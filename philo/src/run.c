#include "philo.h"

int	ft_run(t_table *table, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < table->qty_philo)
	{
		if(pthread_create(&philo->thread, NULL, ft_loop, philo) != 0)
			return (3);
	}
	i = 0;
	while (i < table->qty_philo)
	{
		if(pthread_join(philo->thread, NULL) != 0)
			return (3);
	}
	return (0);
}