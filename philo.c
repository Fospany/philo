/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 16:08:18 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/05/12 14:01:56 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    safe_printf(t_philo *philo, char *message, pthread_mutex_t *data, int lock)
{
    long long int current_time;

    pthread_mutex_lock(&philo->shared_values->write_lock);
    pthread_mutex_lock(&philo->shared_values->death_check);
    current_time = get_time();
    current_time = current_time - philo->shared_values->start_time;
    if (!philo->shared_values->dead)
        printf("%lld %i %s", current_time, philo->id, message);
    pthread_mutex_unlock(&philo->shared_values->death_check);
    pthread_mutex_unlock(&philo->shared_values->write_lock);
    if (lock && data)
        pthread_mutex_lock(data);
    else if (data)
        pthread_mutex_unlock(data);
}

long long int     get_time()
{
    struct timeval  tv;
    
    gettimeofday(&tv, NULL);
    return (((long long)tv.tv_sec * 1000) + ((long long)tv.tv_usec / 1000));
}

void    destroy_every_mutex(t_philo *philos, t_shared *shared)
{
    int i;
    
    i = 0;
    while (i < shared->number_of_philosophers)
    {
        pthread_mutex_destroy(&philos[i].meal_lock);
        pthread_mutex_destroy(&shared->forks[i]);
        i++;
    }
    free(shared->forks);
    pthread_mutex_destroy(&shared->write_lock);
    pthread_mutex_destroy(&shared->max_meals_lock);
    pthread_mutex_destroy(&shared->death_check);
    free(shared);
    free(philos);
}

void    philos(int args, char **argv)
{
    t_shared        *shared;
    t_philo         *philosophers;
    
    shared = malloc(sizeof(t_shared));
    if (!shared)
        return ;
    if(setting_args(args, argv, shared))
        return (free(shared->forks), free(shared));
    philosophers = malloc(sizeof(t_philo) * shared->number_of_philosophers);
    if (!philosophers)
        return (free(shared->forks), free(shared));
    setting_philo_values(philosophers, shared);
    start_the_threads(philosophers, shared);
    destroy_every_mutex(philosophers, shared);
}

int main(int args, char **argv)
{
    philos(args, argv);
    return (0);
}