/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anisabel <anisabel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 23:28:18 by anisabel          #+#    #+#             */
/*   Updated: 2026/07/05 20:45:37 by anisabel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

# define FORK_TAKEN 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define DEAD 5

typedef struct s_table	t_table;

typedef struct s_params
{
	int				eat_duration;
	int				sleep_duration;
	int				think_duration;
	int				die_time;
	long			meal_limit;
	pthread_mutex_t	meal_mutex;
}				t_params;

typedef struct s_philo
{
	int				id;
	t_params		config;
	t_table			*sim;
	pthread_t		thread;
	pthread_mutex_t	fork;
	pthread_mutex_t	time_mutex;
	long			meal_time;
	bool			is_full;
	struct s_philo	*next;
}				t_philo;

struct			s_table
{
	t_philo			*philos;
	int				count;
	bool			active;
	pthread_t		watcher;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	write_mutex;
	long			origin;
};

//UTILS
int		ft_atoi(const char *str);
bool	ft_is_even(int n);
bool	ft_isdigit(int c);
long	ft_atol(const char *str);
bool	parse_args(int ac, char **av, t_table *table);
bool	init_sim(t_table *table, char **av);
void	*philo_cycle(void *arg);
void	log_action(t_philo *philo, int action);
void	*sim_watcher(void *tab);
bool	check_death(t_philo *philo);
void	end_sim(t_table *table);
bool	sim_active(t_table *table);
bool	death_occurred(t_table *table);
bool	all_ate(t_table *table);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);
long	time_ms(void);
void	clear_philos(t_philo **philo);
void	join_all(t_table *table);

#endif