/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:20:10 by mqueiros          #+#    #+#             */
/*   Updated: 2025/08/14 15:40:07 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char			*color;

	table = (t_table){0};
	philo = NULL;
	if (ft_error(parse_args(argc, argv), NULL, NULL))
		return (1);
	if (ft_error(ft_init(argv, &table, &philo), &table, philo))
		return (1);
	if (ft_error(ft_run(&table, philo), &table, philo))
		return (1);
	if (table.qty_eat > 0
		&& table.total_eat != (table.qty_philo * table.qty_eat))
		color = "\033[91m";
	else
		color = "\033[92m";
	printf("\nTotal Meals:%s%d\033[0m\n", color, table.total_eat);
	ft_error(-1, &table, philo);
	return (0);
}
