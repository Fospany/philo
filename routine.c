/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 10:52:03 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/05/18 19:57:03 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*dining(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (philo->shared_values->start_time == 0)
		continue ;
	if (philo->shared_values->number_of_philosophers == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		safe_printf(philo, "has taken a fork\n");
		my_sleep(philo->shared_values->time_to_die);
		safe_printf(philo, "died\n");
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_lock(&philo->shared_values->death_check);
		philo->shared_values->dead = 1;
		pthread_mutex_unlock(&philo->shared_values->death_check);
		return (NULL);
	}
	if (philo->shared_values->number_of_philosophers % 2 == 0)
	{
		if (philo->id % 2 == 0)
			my_sleep(philo->shared_values->time_to_eat / 2);
	}
	else
	{
		if (philo->id % 2 == 0)
			usleep (1000);
	}
	if (philo->id % 2 == 1)
		odd_philo(philo);
	else
	{
		even_philo(philo);
	}
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

int	is_dead(t_shared *shared)
{
	pthread_mutex_lock(&shared->death_check);
	if (shared->dead)
	{
		pthread_mutex_unlock(&shared->death_check);
		return (1);
	}
	pthread_mutex_unlock(&shared->death_check);
	return (0);
}

int	odd_philo(t_philo *philo)
{
	while (1)
	{
		if (is_dead(philo->shared_values))
			return (1);
		wanna_eat_odd(philo);
		if (philo->eaten_meals == philo->shared_values->number_of_meals)
		{
			pthread_mutex_lock(&philo->shared_values->max_meals_lock);
			philo->shared_values->finished_meals++;
			pthread_mutex_unlock(&philo->shared_values->max_meals_lock);
			return (0);
		}
		safe_printf(philo, "is sleeping\n");
		my_sleep(philo->shared_values->time_to_sleep);
		safe_printf(philo, "is thinking\n");
		// pthread_mutex_lock(&philo->shared_values->death_check);
		// if (get_time() - philo->last_meal > philo->shared_values->time_to_die)
		// {
		// 	pthread_mutex_unlock(&philo->shared_values->death_check);
		// 	safe_printf(philo, "died\n");
		// 	pthread_mutex_lock(&philo->shared_values->death_check);
		// 	philo->shared_values->dead = 1;
		// 	pthread_mutex_unlock(&philo->shared_values->death_check);
		// 	return (1);
		// }
		// pthread_mutex_unlock(&philo->shared_values->death_check);
		if (philo->shared_values->number_of_philosophers % 2 == 1)
		{
			long long think_time = (philo->shared_values->time_to_eat * 2) - philo->shared_values->time_to_sleep;
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

int	even_philo(t_philo *philo)
{
	while (1)
	{
		if(is_dead(philo->shared_values))
			return (1);
		wanna_eat_even(philo);
		if (philo->eaten_meals == philo->shared_values->number_of_meals)
		{
			pthread_mutex_lock(&philo->shared_values->max_meals_lock);
			philo->shared_values->finished_meals++;
			pthread_mutex_unlock(&philo->shared_values->max_meals_lock);
			return (0);
		}
		safe_printf(philo, "is sleeping\n");
		my_sleep(philo->shared_values->time_to_sleep);
		safe_printf(philo, "is thinking\n");
		// pthread_mutex_lock(&philo->shared_values->death_check);
		// if (get_time() - philo->last_meal > philo->shared_values->time_to_die)
		// {
		// 	pthread_mutex_lock(&philo->shared_values->write_lock);
		// 	if (philo->shared_values->dead)
		// 	{
		// 		philo->shared_values->dead = 1;
		// 		printf("%lli %i died\n", get_time() - philo->shared_values->start_time, philo->id);
		// 	}
		// 	pthread_mutex_unlock(&philo->shared_values->write_lock);
		// 	pthread_mutex_unlock(&philo->shared_values->death_check);
		// 	return (1);
		// }
		// pthread_mutex_unlock(&philo->shared_values->death_check);
		if (philo->shared_values->number_of_philosophers % 2 == 1)
		{
			long long think_time = (philo->shared_values->time_to_eat * 2) - philo->shared_values->time_to_sleep;
			if (think_time <= 0)
				think_time = 1;
			my_sleep(think_time);
		}
	}
}
