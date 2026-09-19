/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:16:12 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/19 13:31:02 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	reached_max_meals_check(t_philo *philos)//, int i)
{
	int	num_of_philos;
	int	i;
	int finished_meals;
	num_of_philos = philos->shared_values->number_of_philosophers;
	
	finished_meals = 0;
	i = 0;
	while (i < num_of_philos)
	{
		pthread_mutex_lock(&philos[i].meal_lock);
		if (philos[i].eaten_meals == philos[i].shared_values->number_of_meals)
			finished_meals++;
		pthread_mutex_unlock(&philos[i].meal_lock);
		i++;
	}
	if (finished_meals == num_of_philos)
	{
		pthread_mutex_lock(&philos->shared_values->death_check);
		philos->shared_values->dead = 1;
		pthread_mutex_unlock(&philos->shared_values->death_check);
		return (1);
	}
	return (0);
	// pthread_mutex_lock(&philos[i].shared_values->max_meals_lock);
	// if (philos[i].shared_values->finished_meals == num_of_philos)
	// {
	// 	pthread_mutex_lock(&philos[i].shared_values->death_check);
	// 	philos[i].shared_values->dead = 1;
	// 	pthread_mutex_unlock(&philos[i].shared_values->death_check);
	// 	pthread_mutex_unlock(&philos[i].shared_values->max_meals_lock);
	// 	return (1);
	// }
	// pthread_mutex_unlock(&philos[i].shared_values->max_meals_lock);
}

long long int	last_meal_update(t_philo *philos, int i)
{
	long long int	current_meal;

	pthread_mutex_lock(&philos[i].meal_lock);
	current_meal = philos[i].last_meal;
	pthread_mutex_unlock(&philos[i].meal_lock);
	return (current_meal);
}

int	anyone_died_check(t_philo *philos, int i, long long current_meal)
{
	if (get_time() - current_meal >= philos[i].shared_values->time_to_die)
	{
		pthread_mutex_lock(&philos[i].shared_values->death_check);
		philos[i].shared_values->dead = 1;
		pthread_mutex_lock(&philos[i].shared_values->write_lock);
		printf("%lli %i died\n", get_time()
			- philos[i].start_time, philos[i].id);
		pthread_mutex_unlock(&philos[i].shared_values->write_lock);
		pthread_mutex_unlock(&philos[i].shared_values->death_check);
		return (1);
	}
	return (0);
}

int	end_check(t_philo *philos, int i)
{
	long long	current_meal;

	if (reached_max_meals_check(philos))
		return (1);
	current_meal = last_meal_update(philos, i);
	if (anyone_died_check(philos, i, current_meal))
		return (1);
	return (0);
}

void	my_sleep(long long time_to_spend)
{
	long long int	wake_up;

	wake_up = get_time() + time_to_spend;
	while (get_time() <= wake_up)
		usleep(200);
}
