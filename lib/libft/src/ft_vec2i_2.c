/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2i_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:05:21 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 19:01:06 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

t_vec2i	vec2i_f(float x, float y)
{
	t_vec2i	result;

	result.x = (int)x;
	result.y = (int)y;
	return (result);
}
