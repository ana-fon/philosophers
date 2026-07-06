/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:53:13 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/06 17:53:18 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*sim_watcher(void *tab)
{
	t_table	*table;

	table = (t_table *)tab;
	while (1)
	{
		if (death_occurred(table))
			return (NULL);
		if (all_ate(table))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
