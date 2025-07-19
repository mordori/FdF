/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 02:50:31 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/19 02:51:00 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_mat4	isometric_rot(void)
{
	t_mat4	rot_x = mat4_rot_x(atanf(1.0f / sqrtf(2.0f)));
	t_mat4	rot_y = mat4_rot_y(-M_PI / 4.0f);
	return (mat4_mul(rot_x, rot_y));
}
