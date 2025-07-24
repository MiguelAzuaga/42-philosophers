#include "../includes/philo.h"

int	ft_error(int code)
{
	if (code == 42)
		write(2, "Malloc error\n", 13);
	exit(1);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	ans;

	i = 0;
	sign = 1;
	ans = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if ((str[i] == '-' || str[i] == '+'))
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		ans = ans * 10 + (str[i] - '0');
		i++;
	}
	return (sign * ans);
}

void init_args(char **argv, t_philo *args)
{

	args->philo_qty = ft_atoi(argv[1]);
	args->die_t = ft_atoi(argv[2]);
	args->eat_t = ft_atoi(argv[3]);
	args->sleep_t = ft_atoi(argv[4]);
	args->eat_qty = ft_atoi(argv[5]);
	return ;
}

int	main(int argc, char **argv)
{
	t_philo *args;


	if (argc != 6)
		return(printf("Usage:%s <number_of_philosophers> <time_to_die> \
<time_to_eat> <time_to_sleep> <number_of_times_each_philosopher_must_eat>",
		argv[0]));
	args = malloc(sizeof(t_philo));
	if (!args)
			ft_error(42);
	*args = (t_philo){0};
	init_args(argv, args);
	return (0);
}