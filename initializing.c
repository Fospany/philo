/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:00:59 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/05/18 14:17:47 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	setting_philo_values(t_philo *philosophers, t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->number_of_philosophers)
	{
		philosophers[i].shared_values = shared;
		philosophers[i].id = i + 1;
		philosophers[i].last_meal = get_time();
		philosophers[i].eaten_meals = 0;
		if (i == shared->number_of_philosophers - 1)
			philosophers[i].right_fork = &shared->forks[0];
		else
			philosophers[i].right_fork = &shared->forks[i + 1];
		philosophers[i].left_fork = &shared->forks[i];
		pthread_mutex_init(&philosophers[i].meal_lock, NULL);
		i++;
	}
	return (0);
}

int	setting_args(int args, char **argv, t_shared *shared)
{
	int	i;

	shared->finished_meals = 0;
	shared->number_of_philosophers = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	if (args == 6)
		shared->number_of_meals = ft_atoi(argv[5]);
	else
		shared->number_of_meals = -2;
	if (neg_check(shared))
		return (1);
	shared->forks = malloc(sizeof(pthread_mutex_t)
			* shared->number_of_philosophers);
	if (!shared->forks)
		return (1);
	shared->dead = 0;
	i = 0;
	while (i < shared->number_of_philosophers)
		pthread_mutex_init(&shared->forks[i++], NULL);
	pthread_mutex_init(&shared->max_meals_lock, NULL);
	pthread_mutex_init(&shared->write_lock, NULL);
	pthread_mutex_init(&shared->death_check, NULL);
	return (0);
}
