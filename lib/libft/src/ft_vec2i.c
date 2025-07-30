/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2i.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:32:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 19:03:18 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

t_vec2i	vec2i(int x, int y)
{
	t_vec2i	result;

	result.x = x;
	result.y = y;
	return (result);
}

t_vec2i	vec2i_n(float n)
{
	t_vec2i	result;

	result.x = (int)n;
	result.y = (int)n;
	return (result);
}

t_vec2i	vec2i_add(t_vec2i a, t_vec2i b)
{
	t_vec2i	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return (result);
}

t_vec2i	vec2i_sub(t_vec2i a, t_vec2i b)
{
	t_vec2i	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return (result);
}

t_vec2i	vec2i_scale(t_vec2i v, float s)
{
	t_vec2i	result;

	result.x = (int)((float)v.x * s);
	result.y = (int)((float)v.y * s);
	return (result);
}
