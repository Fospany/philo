/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:12:46 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/05/18 13:35:13 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check(char *s, int i)
{
	while (s[i])
	{
		if (!(s[i] < '9' || s[i] > '0'))
			return (-1);
		i++;
	}
	return (0);
}

int	ft_atoi(char *s)
{
	int		i;
	long	res;

	i = 0;
	res = 0;
	if (s[i] == '-')
		return (-1);
	if (s[i] == '+')
		i++;
	if (check(s, i))
		return (-1);
	while (s[i])
	{
		res = res * 10 + (s[i] - '0');
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	return (res);
}

int	neg_check(t_shared *shared)
{
	if (shared->number_of_philosophers == -1)
		return (1);
	if (shared->time_to_die == -1)
		return (1);
	if (shared->time_to_eat == -1)
		return (1);
	if (shared->time_to_die == -1)
		return (1);
	if (shared->number_of_meals == -1)
		return (1);
	return (0);
}
