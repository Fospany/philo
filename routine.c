/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 10:52:03 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/05/19 13:49:28 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*dining(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (wait_for_others(philo))
		continue ;
	if (philo->shared_values->number_of_philosophers == 1)
		return (only_one_philo(philo), NULL);
	schedule_sleeps(philo);
	if (philo->id % 2 == 1)
		odd_philo(philo);
	else
		even_philo(philo);
	return (NULL);
}

void	wanna_eat_odd(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_printf(philo, "has taken a fork\n");
	if (is_dead(philo->shared_values))
	{
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	safe_printf(philo, "has taken a fork\n");
	safe_printf(philo, "is eating\n");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	if (philo->shared_values->number_of_meals != -2)
		philo->eaten_meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	my_sleep(philo->shared_values->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	odd_philo(t_philo *philo)
{
	long long	think_time;

	while (1)
	{
		if (is_dead(philo->shared_values))
			return ;
		wanna_eat_odd(philo);
		if (finished_eating_check(philo))
			return ;
		safe_printf(philo, "is sleeping\n");
		my_sleep(philo->shared_values->time_to_sleep);
		safe_printf(philo, "is thinking\n");
		if (philo->shared_values->number_of_philosophers % 2 == 1)
		{
			think_time = (philo->shared_values->time_to_eat * 2)
				- philo->shared_values->time_to_sleep;
			if (think_time <= 0)
				think_time = 1;
			my_sleep(think_time);
		}
	}
}

void	wanna_eat_even(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	safe_printf(philo, "has taken a fork\n");
	if (is_dead(philo->shared_values))
	{
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	pthread_mutex_lock(philo->left_fork);
	safe_printf(philo, "has taken a fork\n");
	safe_printf(philo, "is eating\n");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_lock);
	if (philo->shared_values->number_of_meals != -2)
		philo->eaten_meals++;
	my_sleep(philo->shared_values->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	even_philo(t_philo *philo)
{
	long long	think_time;

	while (1)
	{
		if (is_dead(philo->shared_values))
			return ;
		wanna_eat_even(philo);
		if (finished_eating_check(philo))
			return ;
		safe_printf(philo, "is sleeping\n");
		my_sleep(philo->shared_values->time_to_sleep);
		safe_printf(philo, "is thinking\n");
		if (philo->shared_values->number_of_philosophers % 2 == 1)
		{
			think_time = (philo->shared_values->time_to_eat * 2)
				- philo->shared_values->time_to_sleep;
			if (think_time <= 0)
				think_time = 1;
			my_sleep(think_time);
		}
	}
}
