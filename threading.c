/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 10:52:03 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/05/14 10:27:32 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int     end_check(t_philo *philos, int i)
{
    pthread_mutex_lock(&philos[i].shared_values->max_meals_lock);
    if (philos->shared_values->finished_meals == philos->shared_values->number_of_philosophers)
    {
        pthread_mutex_lock(&philos->shared_values->death_check);
        printf("Every philosopher finished eating\n");
        philos->shared_values->dead = 1;
        pthread_mutex_unlock(&philos->shared_values->death_check);
        pthread_mutex_unlock(&philos->shared_values->max_meals_lock);
        return (1);
    }
    pthread_mutex_unlock(&philos[i].shared_values->max_meals_lock);
    pthread_mutex_lock(&philos[i].meal_lock);
    if (philos->shared_values->time_to_die < get_time() - philos->shared_values->start_time - philos[i].last_meal)
    {
        pthread_mutex_lock(&philos->shared_values->death_check);
        philos->shared_values->dead = 1;
        pthread_mutex_unlock(&philos->shared_values->death_check);
        printf("%lli %i died\n", get_time() - philos->shared_values->start_time, philos[i].id);
        pthread_mutex_unlock(&philos[i].meal_lock);
        return (1);
    }
    pthread_mutex_unlock(&philos[i].meal_lock);
    return (0);
}

void    *monitoring(void *ptr)
{
    t_philo *philos;
    int      i;
    
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
    }
    return (NULL);
}

void    wanna_eat_odd(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
    safe_printf(philo, "has taken a fork\n", NULL, -1);
    pthread_mutex_lock(philo->right_fork);
    safe_printf(philo, "has taken another fork\n", &philo->meal_lock, 1);
    safe_printf(philo, "is eating\n", NULL, -1);
    philo->last_meal = get_time() - philo->shared_values->start_time;
    if (philo->shared_values->number_of_meals != -1)
        philo->eaten_meals++;
    pthread_mutex_unlock(&philo->meal_lock);
    usleep(philo->shared_values->time_to_eat * 1000);
    safe_printf(philo, "finished eating\n", philo->left_fork, 0);
    pthread_mutex_unlock(philo->right_fork);
}

int    odd_philo(t_philo *philo)
{
    while (1)
    {
        pthread_mutex_lock(&philo->shared_values->death_check);
        if (philo->shared_values->dead)
            return(pthread_mutex_unlock(&philo->shared_values->death_check));
        pthread_mutex_unlock(&philo->shared_values->death_check);
        wanna_eat_odd(philo);
        if (philo->eaten_meals == philo->shared_values->number_of_meals)
        {
            pthread_mutex_lock(&philo->shared_values->max_meals_lock);
            philo->shared_values->finished_meals++;
            pthread_mutex_unlock(&philo->shared_values->max_meals_lock);
            return (0);
        }
        safe_printf(philo, "is sleeping\n", NULL, -1);
        usleep(philo->shared_values->time_to_sleep * 1000);
        safe_printf(philo, "is thinking\n", NULL, -1);
    }
}

void    wanna_eat_even(t_philo *philo)
{
    pthread_mutex_lock(philo->right_fork);
    safe_printf(philo, "has taken a fork\n", NULL, -1);
    pthread_mutex_lock(philo->left_fork);
    safe_printf(philo, "has taken another fork\n", &philo->meal_lock, 1);
    safe_printf(philo, "is eating\n", NULL, -1);
    philo->last_meal = get_time() - philo->shared_values->start_time;
    if (philo->shared_values->number_of_meals != -1)
        philo->eaten_meals++;
    pthread_mutex_unlock(&philo->meal_lock);
    usleep(philo->shared_values->time_to_eat * 1000);
    safe_printf(philo, "finished eating\n", philo->right_fork, 0);
    pthread_mutex_unlock(philo->left_fork);
}

int    even_philo(t_philo *philo)
{
    while (1)
    {
        pthread_mutex_lock(&philo->shared_values->death_check);
        if (philo->shared_values->dead)
            return(pthread_mutex_unlock(&philo->shared_values->death_check));
        pthread_mutex_unlock(&philo->shared_values->death_check);
        wanna_eat_even(philo);
        if (philo->eaten_meals == philo->shared_values->number_of_meals)
        {
            pthread_mutex_lock(&philo->shared_values->max_meals_lock);
            philo->shared_values->finished_meals++;
            pthread_mutex_unlock(&philo->shared_values->max_meals_lock);
            return (0);
        }
        safe_printf(philo, "is sleeping\n", NULL, -1);
        usleep(philo->shared_values->time_to_sleep * 1000);
        safe_printf(philo, "is thinking\n", NULL, -1);
    }
}
