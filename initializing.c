/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:00:59 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/05/12 14:02:29 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int    setting_philo_values(t_philo *philosophers, t_shared *shared)
{
    int i;
    
    i = 0;
    while (i < shared->number_of_philosophers)
    {
        philosophers[i].shared_values = shared;
        philosophers[i].id = i + 1;
        philosophers[i].last_meal = 0;
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

int    setting_args(int args, char **argv, t_shared *shared)
{
    int i;
    
    shared->finished_meals = 0;
    shared->number_of_philosophers = ft_atoi(argv[1]);
    shared->time_to_die = ft_atoi(argv[2]);
    shared->time_to_eat = ft_atoi(argv[3]);
    shared->time_to_sleep = ft_atoi(argv[4]);
    shared->forks = malloc(sizeof(pthread_mutex_t) * shared->number_of_philosophers);
    if (!shared->forks)
        return (1);
    if (args == 6)
        shared->number_of_meals = ft_atoi(argv[5]);
    else
        shared->number_of_meals = -1;
    shared->dead = 0;
    i = 0;
    while (i < shared->number_of_philosophers)
    {
        pthread_mutex_init(&shared->forks[i], NULL);
        i++;
    }
    pthread_mutex_init(&shared->max_meals_lock, NULL);
    pthread_mutex_init(&shared->write_lock, NULL);
    pthread_mutex_init(&shared->death_check, NULL);
    return (0);
}

void    start_the_threads(t_philo *philos, t_shared *shared)
{
    int i;
    pthread_t *threads;
    pthread_t monitor;
    
    threads = malloc(sizeof(pthread_t) * shared->number_of_philosophers);
    if (!threads)
        return ;
    i = 0;
    shared->start_time = get_time();
    while (i < shared->number_of_philosophers)
    {
        pthread_create(&threads[i], NULL, &dining, &philos[i]);
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

void    *dining(void *ptr)
{
    t_philo *philo;
    
    philo = (t_philo *)ptr;
    if (philo->shared_values->number_of_philosophers == 1)
    {
        printf("1 kocsog van csak\n");
        pthread_mutex_lock(philo->left_fork);
        safe_printf(philo, "has taken a fork\n", NULL, -1);
        usleep(philo->shared_values->time_to_die * 1000);
        pthread_mutex_unlock(philo->left_fork);
        return (NULL);
    }
    if (philo->id % 2 == 1)
        odd_philo(philo);
    else
        even_philo(philo);
    return (NULL);
}
