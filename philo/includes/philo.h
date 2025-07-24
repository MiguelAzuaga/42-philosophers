#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_philo
{
	int	philo_qty;
	int	die_t;
	int	eat_t;
	int	sleep_t;
	int	eat_qty;
}	t_philo;


#endif