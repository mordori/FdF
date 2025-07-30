/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:28:02 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/22 16:54:09 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

float	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	vec3_normalize(t_vec3 v)
{
	float	len;

	len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x /= len;
	v.y /= len;
	v.z /= len;
	return (v);
}

t_vec3	vec3_4(t_vec4 v)
{
	t_vec3	result;

	result.x = v.x;
	result.y = v.y;
	result.z = v.z;
	return (result);
}

float	vec3_length(t_vec3 v)
{
	return (sqrt(vec3_dot(v, v)));
}
