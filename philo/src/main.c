#include "../includes/philo.h"
#include <sys/time.h>

void	erro_msg(int error)
{
	if (error == MALLOC)
		write(2, "Malloc error\n", 13);
	else if (error == USAGE)
		write(2, "Usage:./philo <number_of_philosophers> <time_to_die> <time_\
to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n", 127);
	else if (error == INVALID_ARGS)
		write(2, "Invalid arguments\n", 18);
	else if (error == MUTEX)
		write(2, "Mutex error\n", 12);
	return ;
}

void	ft_error(int error, t_data *data)
{
	if (error == 0)
		return ;
	if (data)
		free(data);
	erro_msg(error);
	exit(EXIT_FAILURE);
}

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
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

int	init_args(char **argv, t_data *data)
{
	data->qty_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	data->qty_eat = -1;
	data->start_time = get_current_time_ms();
	if (argv[5])
		data->qty_eat = ft_atoi(argv[5]);
	if (data->qty_philo == 0 || data->qty_eat == 0)
		return (INVALID_ARGS);
	if (pthread_mutex_init(&data->mutex, NULL) != 0)
		return (MUTEX);
	return (0);
}

int	parse_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
		return (USAGE);
	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (INVALID_ARGS);
			j++;
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philo;

	ft_error(parse_args(argc, argv), NULL);
	ft_error(init_args, NULL);
	return (0);
}
