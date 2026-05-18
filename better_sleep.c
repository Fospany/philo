/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   better_sleep.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 13:48:18 by bguthy            #+#    #+#             */
/*   Updated: 2026/05/18 16:44:13 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	my_sleep(long long time_to_spend)
{
	long long int	wake_up;

	wake_up = get_time() + time_to_spend;
	while (get_time() < wake_up)
		usleep(200);
}
