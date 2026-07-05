/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 23:25:35 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/02 23:29:31 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	long	n;

	n = 0;
	while (*str >= '0' && *str <= '9')
		n = n * 10 + (*str++ - '0');
	return ((int)n);
}

long	ft_atol(const char *str)
{
	long	n;

	n = 0;
	while (*str >= '0' && *str <= '9')
		n = n * 10 + (*str++ - '0');
	return (n);
}

bool	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

bool	ft_is_even(int n)
{
	return (n % 2 == 0);
}

long	time_ms(void)
{
	long			time;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return (time);
}

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
