/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 23:25:24 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/02 23:27:09 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

static int	smart_sleep(int ms, t_philo *philo)
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

static void	dine(t_philo *philo, int *meals_eaten)
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

static void	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	log_action(philo, FORK_TAKEN);
	while (sim_active(philo->sim))
		usleep(100);
	pthread_mutex_unlock(&philo->fork);
}

static int	check_philo_full(t_philo *philo)
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

static void	extra_think_time(t_philo *philo)
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
