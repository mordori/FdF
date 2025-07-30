/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:46:02 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/19 23:34:27 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

t_vec4	vec4(float x, float y, float z, float w)
{
	t_vec4	result;

	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;
	return (result);
}

t_vec4	vec4_n(float n)
{
	t_vec4	result;

	result.x = n;
	result.y = n;
	result.z = n;
	result.w = n;
	return (result);
}

t_vec4	vec4_add(t_vec4 a, t_vec4 b)
{
	t_vec4	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;
	return (result);
}

t_vec4	vec4_sub(t_vec4 a, t_vec4 b)
{
	t_vec4	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	result.w = a.w - b.w;
	return (result);
}

t_vec4	vec4_scale(t_vec4 v, float s)
{
	t_vec4	result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	result.w = v.w * s;
	return (result);
}
