/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dining.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:52:45 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/06 18:57:17 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	smart_sleep(int ms, t_philo *philo)
{
	long	start;

	start = time_ms();
	while (time_ms() - start < ms)
	{
		if (!sim_active(philo->sim))
			return (0);
		usleep(100);
	}
	return (1);
}

void	dine(t_philo *philo, int *meals_eaten)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->time_mutex);
	philo->meal_time = time_ms();
	pthread_mutex_unlock(&philo->time_mutex);
	log_action(philo, EAT);
	smart_sleep(philo->config.eat_duration, philo);
	if (philo->config.meal_limit != -1)
	{
		(*meals_eaten)++;
		if (*meals_eaten >= philo->config.meal_limit)
		{
			pthread_mutex_lock(&philo->config.meal_mutex);
			philo->is_full = true;
			pthread_mutex_unlock(&philo->config.meal_mutex);
		}
	}
	drop_forks(philo);
}

void	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	log_action(philo, FORK_TAKEN);
	while (sim_active(philo->sim))
		usleep(100);
	pthread_mutex_unlock(&philo->fork);
}

int	check_philo_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->config.meal_mutex);
	if (philo->is_full)
	{
		pthread_mutex_unlock(&philo->config.meal_mutex);
		while (sim_active(philo->sim))
			usleep(100);
		return (1);
	}
	pthread_mutex_unlock(&philo->config.meal_mutex);
	return (0);
}

void	extra_think_time(t_philo *philo)
{
	long	think_time;

	if (philo->sim->count % 2 != 0)
	{
		think_time = (philo->config.die_time
				- (philo->config.eat_duration
					+ philo->config.sleep_duration)) / 2;
		if (think_time < 0)
			think_time = 0;
		smart_sleep(think_time, philo);
	}
	else
		usleep(200);
}
