/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 23:25:22 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/06 16:43:46 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	check_death(t_philo *philo)
{
	long	elapsed;

	pthread_mutex_lock(&philo->time_mutex);
	elapsed = time_ms() - philo->meal_time;
	pthread_mutex_unlock(&philo->time_mutex);
	return (elapsed > philo->config.die_time);
}

void	end_sim(t_table *table)
{
	pthread_mutex_lock(&table->state_mutex);
	table->active = false;
	pthread_mutex_unlock(&table->state_mutex);
}

bool	sim_active(t_table *table)
{
	bool	running;

	pthread_mutex_lock(&table->state_mutex);
	running = table->active;
	pthread_mutex_unlock(&table->state_mutex);
	return (running);
}

bool	death_occurred(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->philos;
	i = 0;
	while (i < table->count)
	{
		if (check_death(philo))
		{
			log_action(philo, DEAD);
			end_sim(table);
			return (true);
		}
		philo = philo->next;
		i++;
	}
	return (false);
}

bool	all_ate(t_table *table)
{
	t_philo		*philo;
	int			i;

	if (table->philos->config.meal_limit == -1)
		return (false);
	philo = table->philos;
	i = 0;
	while (i < table->count)
	{
		pthread_mutex_lock(&philo->config.meal_mutex);
		if (!philo->is_full)
		{
			pthread_mutex_unlock(&philo->config.meal_mutex);
			return (false);
		}
		pthread_mutex_unlock(&philo->config.meal_mutex);
		philo = philo->next;
		i++;
	}
	end_sim(table);
	return (true);
}


