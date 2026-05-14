/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:12:46 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/05/12 12:07:06 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int     check(char *s, int i)
{
    while (s[i])
    {
        if (!(s[i] < '9' || s[i] > '0'))
            return (-1);
        i++;
    }
    return (0);
}

int ft_atoi(char *s)
{
    int i;
    long res;

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
