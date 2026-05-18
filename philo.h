/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 13:15:58 by bguthy            #+#    #+#             */
/*   Updated: 2026/05/18 19:39:28 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_shared
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_meals;
	int				dead;
	long long int	start_time;
	int				finished_meals;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_check;
	pthread_mutex_t	max_meals_lock;
}					t_shared;

typedef struct s_philo
{
	int				id;
	long long int	last_meal;
	int				eaten_meals;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_shared		*shared_values;
}					t_philo;

int					check(char *s, int i);
int					ft_atoi(char *s);
void				safe_printf(t_philo *philo, char *message);
int					setting_args(int args, char **argv, t_shared *shared);
int					setting_philo_values(t_philo *philosophers,
						t_shared *shared);
long long int		get_time(void);
int					odd_philo(t_philo *philo);
int					even_philo(t_philo *philo);
void				*dining(void *ptr);
void				*monitoring(void *ptr);
void				start_the_threads(t_philo *philos, t_shared *shared);
int					philos(int args, char **argv);
int					neg_check(t_shared *shared);
void				my_sleep(long long time_to_spend);
int					is_dead(t_shared *shared);

#endif