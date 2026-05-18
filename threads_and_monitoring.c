/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_and_monitoring.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 13:23:59 by bguthy            #+#    #+#             */
/*   Updated: 2026/05/18 20:12:27 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	end_check(t_philo *philos, int i, int num_of_philos)
{
	pthread_mutex_lock(&philos[i].shared_values->max_meals_lock);
	if (philos[i].shared_values->finished_meals == num_of_philos)
	{
		pthread_mutex_lock(&philos->shared_values->death_check);
		philos->shared_values->dead = 1;
		printf("Every philosopher finished eating\n");
		pthread_mutex_unlock(&philos->shared_values->death_check);
		pthread_mutex_unlock(&philos->shared_values->max_meals_lock);
		return (1);
	}
	pthread_mutex_unlock(&philos[i].shared_values->max_meals_lock);
	int w = 0;
	long long current_meal;
	while (w < philos->shared_values->number_of_philosophers)
	{
		pthread_mutex_lock(&philos[i].meal_lock);
		current_meal = philos[i].last_meal;
		pthread_mutex_unlock(&philos[i].meal_lock);
		pthread_mutex_lock(&philos->shared_values->death_check);
		if (get_time() - current_meal > philos->shared_values->time_to_die)
		{
			pthread_mutex_lock(&philos->shared_values->write_lock);
			printf("%lli %i died\n", get_time() - philos->shared_values->start_time, philos[i].id);
			philos->shared_values->dead = 1;
			pthread_mutex_unlock(&philos->shared_values->write_lock);
			pthread_mutex_unlock(&philos->shared_values->death_check);
			return (1);
		}
		pthread_mutex_unlock(&philos->shared_values->death_check);
		w++;
	}
	return (0);
}

void	*monitoring(void *ptr)
{
	t_philo	*philos;
	int		i;
	int		n_of_philos;

	i = 0;
	philos = (t_philo *)ptr;
	n_of_philos = philos->shared_values->number_of_philosophers;
	while (1)
	{
		while (i < philos->shared_values->number_of_philosophers)
		{
			if (end_check(philos, i, n_of_philos))
				return (NULL);
			i++;
		}
		i = 0;
		usleep(1700);
	}
	return (NULL);
}

void	start_the_threads(t_philo *philos, t_shared *shared)
{
	int			i;
	pthread_t	*threads;
	pthread_t	monitor;

	threads = malloc(sizeof(pthread_t) * shared->number_of_philosophers);
	if (!threads)
		return ;
	i = 0;
	shared->start_time = 0;
	while (i < shared->number_of_philosophers)
	{
		pthread_create(&threads[i], NULL, &dining, &philos[i]);
		i++;
	}
	shared->start_time = get_time();
	i = 0;
	while (i < shared->number_of_philosophers)
	{
		philos[i].last_meal = shared->start_time;
		i++;
	}
	pthread_create(&monitor, NULL, &monitoring, philos);
	i = 0;
	while (i < shared->number_of_philosophers)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	free(threads);
}
