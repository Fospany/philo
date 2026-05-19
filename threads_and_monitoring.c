/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_and_monitoring.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 13:23:59 by bguthy            #+#    #+#             */
/*   Updated: 2026/05/19 12:51:42 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitoring(void *ptr)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = (t_philo *)ptr;
	while (1)
	{
		while (i < philos->shared_values->number_of_philosophers)
		{
			if (end_check(philos, i))
				return (NULL);
			i++;
		}
		i = 0;
		usleep(250);
	}
	return (NULL);
}

void	setting_start_times(t_philo *philos, t_shared *shared)
{
	int				i;
	long long int	local_start_time;

	i = 0;
	pthread_mutex_lock(&shared->start_lock);
	shared->start_time = get_time();
	local_start_time = shared->start_time;
	pthread_mutex_unlock(&shared->start_lock);
	while (i < shared->number_of_philosophers)
	{
		pthread_mutex_lock(&philos[i].meal_lock);
		philos[i].last_meal = local_start_time;
		pthread_mutex_unlock(&philos[i].meal_lock);
		i++;
	}
}

void	thread_clean_up(pthread_t *threads, int i, t_shared *shared)
{
	int	j;

	j = 0;
	pthread_mutex_lock(&shared->death_check);
	shared->dead = 1;
	pthread_mutex_unlock(&shared->death_check);
	while (j < i)
	{
		pthread_join(threads[i], NULL);
		j++;
	}
}

void	finishing_threads(pthread_t monitor, pthread_t *threads, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	free(threads);
}

void	start_the_threads(t_philo *philos, t_shared *shared)
{
	int				i;
	pthread_t		*threads;
	pthread_t		monitor;

	threads = malloc(sizeof(pthread_t) * shared->number_of_philosophers);
	if (!threads)
		return ;
	i = 0;
	shared->start_time = 0;
	while (i < shared->number_of_philosophers)
	{
		if (pthread_create(&threads[i], NULL, &dining, &philos[i]) != 0)
			return (thread_clean_up(threads, i, shared));
		i++;
	}
	setting_start_times(philos, shared);
	pthread_create(&monitor, NULL, &monitoring, philos);
	finishing_threads(monitor, threads, shared->number_of_philosophers);
}
