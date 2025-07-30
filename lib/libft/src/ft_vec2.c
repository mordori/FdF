/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 01:30:53 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/20 01:35:20 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

t_vec2	vec2(float x, float y)
{
	t_vec2	result;

	result.x = x;
	result.y = y;
	return (result);
}

t_vec2	vec2_n(float n)
{
	t_vec2	result;

	result.x = n;
	result.y = n;
	return (result);
}

t_vec2	vec2_add(t_vec2 a, t_vec2 b)
{
	t_vec2	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return (result);
}

t_vec2	vec2_sub(t_vec2 a, t_vec2 b)
{
	t_vec2	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return (result);
}

t_vec2	vec2_scale(t_vec2 v, float s)
{
	t_vec2	result;

	result.x = v.x * s;
	result.y = v.y * s;
	return (result);
}
