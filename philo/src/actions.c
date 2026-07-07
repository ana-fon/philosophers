/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 23:25:24 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/07 14:03:34 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static const char	*status_msg(int action)
{
	if (action == FORK_TAKEN)
		return ("has taken a fork");
	if (action == EAT)
		return ("is eating");
	if (action == SLEEP)
		return ("is sleeping");
	if (action == THINK)
		return ("is thinking");
	if (action == DEAD)
		return ("died");
	return (NULL);
}

void	log_action(t_philo *philo, int action)
{
	pthread_mutex_lock(&philo->sim->write_mutex);
	if (sim_active(philo->sim) || action == DEAD)
	{
		printf("%ld %d %s\n",
			time_ms() - philo->sim->origin,
			philo->id,
			status_msg(action));
	}
	pthread_mutex_unlock(&philo->sim->write_mutex);
}

void	take_forks(t_philo *philo)
{
	if (ft_is_even(philo->id))
	{
		pthread_mutex_lock(&philo->fork);
		log_action(philo, FORK_TAKEN);
		pthread_mutex_lock(&philo->next->fork);
		log_action(philo, FORK_TAKEN);
	}
	else
	{
		pthread_mutex_lock(&philo->next->fork);
		log_action(philo, FORK_TAKEN);
		pthread_mutex_lock(&philo->fork);
		log_action(philo, FORK_TAKEN);
	}
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->next->fork);
}

void	*philo_cycle(void *arg)
{
	t_philo	*philo;
	int		meals_eaten;

	philo = (t_philo *)arg;
	meals_eaten = 0;
	pthread_mutex_lock(&philo->time_mutex);
	philo->meal_time = time_ms();
	pthread_mutex_unlock(&philo->time_mutex);
	if (philo->sim->count == 1)
		return (handle_single_philo(philo), NULL);
	if (ft_is_even(philo->id))
		usleep(1000);
	while (sim_active(philo->sim))
	{
		dine(philo, &meals_eaten);
		if (check_philo_full(philo))
			break ;
		log_action(philo, SLEEP);
		if (!smart_sleep(philo->config.sleep_duration, philo))
			break ;
		log_action(philo, THINK);
		extra_think_time(philo);
	}
	return (NULL);
}
