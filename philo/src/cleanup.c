/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 16:55:00 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/07 14:03:41 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	clear_philos(t_philo **philo)
{
	t_philo	*cur;
	t_philo	*next;

	if (!philo || !*philo)
		return ;
	cur = *philo;
	while (cur->next != *philo)
		cur = cur->next;
	cur->next = NULL;
	cur = *philo;
	while (cur)
	{
		next = cur->next;
		pthread_mutex_destroy(&cur->fork);
		pthread_mutex_destroy(&cur->time_mutex);
		pthread_mutex_destroy(&cur->config.meal_mutex);
		free(cur);
		cur = next;
	}
	*philo = NULL;
}

void	join_all(t_table *table)
{
	t_philo	*philo;
	int		i;

	pthread_join(table->watcher, NULL);
	philo = table->philos;
	i = 0;
	while (i < table->count)
	{
		pthread_join(philo->thread, NULL);
		philo = philo->next;
		i++;
	}
}
