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

static bool	all_digits(const char *s)
{
	if (!*s)
		return (false);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	parse_args(int ac, char **av, t_table *table)
{
	int	i;

	if (ac < 5 || ac > 6)
	{
		printf("Usage: %s number_of_philosophers ", av[0]);
		printf("time_to_die time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (false);
	}
	i = 1;
	while (i < ac)
	{
		if (!all_digits(av[i]))
		{
			printf("Error: arguments must be positive integers\n");
			return (false);
		}
		i++;
	}
	table->count = ft_atoi(av[1]);
	if (table->count <= 0)
		return (printf("Error: philosophers count must be positive\n"), false);
	return (true);
}

int	main(int ac, char **av)
{
	t_table	table;
	t_philo	*philo;
	int		i;

	if (!parse_args(ac, av, &table))
		return (1);
	if (!init_sim(&table, av))
		return (1);
	philo = table.philos;
	i = 0;
	while (i < table.count)
	{
		if (pthread_create(&philo->thread, NULL, philo_cycle, philo))
			return (1);
		philo = philo->next;
		i++;
	}
	if (pthread_create(&table.watcher, NULL, sim_watcher, &table))
		return (1);
	join_all(&table);
	clear_philos(&table.philos);
	pthread_mutex_destroy(&table.write_mutex);
	pthread_mutex_destroy(&table.state_mutex);
}
