/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:05:34 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/19 12:42:57 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	only_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_printf(philo, "has taken a fork\n");
	my_sleep(philo->shared_values->time_to_die);
	pthread_mutex_unlock(philo->left_fork);
}

int	wait_for_others(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared_values->start_lock);
	if (philo->shared_values->start_time != 0)
	{
		pthread_mutex_unlock(&philo->shared_values->start_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->shared_values->start_lock);
	usleep(50);
	return (1);
}

int	finished_eating_check(t_philo *philo)
{
	int done;
	
	done = 0;
	pthread_mutex_lock(&philo->meal_lock);
	if (philo->eaten_meals == philo->shared_values->number_of_meals)
		done =1;
	pthread_mutex_unlock(&philo->meal_lock);
	if (done)
		return (1);
	return (0);
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
