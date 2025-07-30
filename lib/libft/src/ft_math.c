/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:14:46 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/29 19:40:03 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

float	ft_lerp(float a, float b, float t)
{
	return ((1.0f - t) * a + t * b);
}

int	ft_imax(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	ft_imin(int a, int b)
{
	if (a > b)
		return (b);
	return (a);
}
