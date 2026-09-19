/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 10:52:03 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/09/19 13:39:53 by guthybarnak      ###   ########.fr       */
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
	if (philo->id % 2 == 1)
		odd_philo(philo);
	else
		even_philo(philo);
	return (NULL);
}

void	wanna_eat_odd(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (is_dead(philo->shared_values))
		return (let_go_of_left_fork(philo));
	safe_printf(philo, "has taken a fork\n");
	pthread_mutex_lock(philo->right_fork);
	if (is_dead(philo->shared_values))
		return (let_go_of_forks(philo));
	safe_printf(philo, "has taken a fork\n");
	safe_printf(philo, "is eating\n");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	if (philo->shared_values->number_of_meals != -2)
		philo->eaten_meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	my_sleep(philo->shared_values->time_to_eat);
	let_go_of_forks(philo);
}

void	think_a_little(t_philo *philo)
{
	long long int think_time;

	think_time = philo->shared_values->time_to_eat * 2 - philo->shared_values->time_to_sleep;
	if (think_time > 0)
		my_sleep(think_time);
}

void	odd_philo(t_philo *philo)
{
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
			think_a_little(philo);
	}
}

void	let_go_of_right_fork(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
}

void	let_go_of_left_fork(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
}

void	let_go_of_forks(t_philo *philo)
{
	let_go_of_left_fork(philo);
	let_go_of_right_fork(philo);
}

void	wanna_eat_even(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (is_dead(philo->shared_values))
		return (let_go_of_right_fork(philo));
	safe_printf(philo, "has taken a fork\n");
	pthread_mutex_lock(philo->left_fork);
	if (is_dead(philo->shared_values))
		return (let_go_of_forks(philo));
	safe_printf(philo, "has taken a fork\n");
	safe_printf(philo, "is eating\n");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	if (philo->shared_values->number_of_meals != -2)
		philo->eaten_meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	my_sleep(philo->shared_values->time_to_eat);
	let_go_of_forks(philo);
}

void	even_philo(t_philo *philo)
{
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
			think_a_little(philo);
	}
}
