/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 23:25:37 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/02 23:30:57 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_init(t_table *table, t_philo *philo, int id, char **av)
{
	philo->id = id;
	philo->sim = table;
	philo->meal_time = table->origin;
	philo->is_full = false;
	philo->config.die_time = ft_atoi(av[2]);
	philo->config.eat_duration = ft_atoi(av[3]);
	philo->config.sleep_duration = ft_atoi(av[4]);
	philo->config.think_duration = 0;
}

static bool	setup_philo(t_table *table, t_philo *philo, int id, char **av)
{
	philo_init(table, philo, id, av);
	if (av[5])
		philo->config.meal_limit = ft_atoi(av[5]);
	else
		philo->config.meal_limit = -1;
	philo->next = NULL;
	if (pthread_mutex_init(&philo->fork, NULL))
		return (false);
	if (pthread_mutex_init(&philo->time_mutex, NULL))
	{
		pthread_mutex_destroy(&philo->fork);
		return (false);
	}
	if (pthread_mutex_init(&philo->config.meal_mutex, NULL))
	{
		pthread_mutex_destroy(&philo->fork);
		pthread_mutex_destroy(&philo->time_mutex);
		return (false);
	}
	return (true);
}

static void	init_cleanup(t_table *table)
{
	clear_philos(&table->philos);
	pthread_mutex_destroy(&table->write_mutex);
	pthread_mutex_destroy(&table->state_mutex);
}

static bool	create_philos(t_table *table, char **av)
{
	t_philo	*philo;
	t_philo	*prev;
	int		i;

	i = 0;
	prev = NULL;
	while (i < table->count)
	{
		philo = malloc(sizeof(t_philo));
		if (!philo || !setup_philo(table, philo, ++i, av))
			return (free(philo), init_cleanup(table), false);
		if (!prev)
			table->philos = philo;
		else
			prev->next = philo;
		prev = philo;
	}
	if (table->count > 0)
		prev->next = table->philos;
	return (true);
}

bool	init_sim(t_table *table, char **av)
{
	if (pthread_mutex_init(&table->write_mutex, NULL)
		|| pthread_mutex_init(&table->state_mutex, NULL))
		return (pthread_mutex_destroy(&table->write_mutex), false);
	table->origin = time_ms();
	table->active = true;
	table->philos = NULL;
	if (!create_philos(table, av))
		return (false);
	return (true);
}
