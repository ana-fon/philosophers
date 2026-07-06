/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 23:25:29 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/05 21:05:22 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	valid_arg(const char *s)
{
	int		len;
	long	val;

	len = 0;
	if (!*s)
		return (false);
	while (s[len])
	{
		if (!ft_isdigit(s[len]))
			return (false);
		len++;
	}
	if (len > 10)
		return (false);
	val = ft_atol(s);
	return (val > 0 && val <= INT_MAX);
}

bool	parse_args(int ac, char **av, t_table *table)
{
	int	i;

	if (ac < 5 || ac > 6)
	{
		printf("Error: invalid number of arguments\n");
		return (false);
	}
	i = 0;
	while (++i < ac)
	{
		if (!valid_arg(av[i]))
		{
			printf("Error: invalid argument value\n");
			return (false);
		}
	}
	table->count = ft_atoi(av[1]);
	return (true);
}

static bool	start_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->philos;
	i = 0;
	while (i < table->count)
	{
		if (pthread_create(&philo->thread, NULL, philo_cycle, philo))
			return (false);
		philo = philo->next;
		i++;
	}
	if (pthread_create(&table->watcher, NULL, sim_watcher, table))
		return (false);
	return (true);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (!parse_args(ac, av, &table))
		return (1);
	if (!init_sim(&table, av))
		return (1);
	if (!start_threads(&table))
		return (1);
	join_all(&table);
	clear_philos(&table.philos);
	pthread_mutex_destroy(&table.write_mutex);
	pthread_mutex_destroy(&table.state_mutex);
}
