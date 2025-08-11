#include "philo.h"

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
	t_table			table;
	t_philo			*philo;

	table = (t_table){0};
	philo = NULL;
	ft_error(parse_args(argc, argv), NULL, NULL);
	ft_error(ft_init(argv, &table, &philo), &table, philo);
	ft_error(ft_run(&table, philo), &table, philo);
	ft_error(-1, &table, philo);
	return (0);
}
