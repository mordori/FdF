/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   depth.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:08:22 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/29 12:51:22 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Performs a depth test for a pixel and updates the z-buffer if visible.
 *
 * Interpolates the depth of the current pixel along the line
 * segment between `v0` and `v1`, using the interpolation parameter `t.x`.
 * The computed depth is compared against the z-buffer at the (x, y)
 * screen coordinates.
 *
 * If the pixel is closer than the current value in the z-buffer,
 * the function updates the z-buffer and sets the color to be drawn.
 *
 * @param ctx   Rendering context containing the z-buffer and color settings.
 * @param v0    Starting vertex of the segment.
 * @param v1    Ending vertex of the segment.
 * @param t     Interpolation parameters.
 * @return      True if the pixel passes the depth test and should be drawn.
 */
bool	depth_test(t_context *ctx, t_vertex v0, t_vertex v1, t_vec3 t)
{
	float	depth;
	int		index;

	depth = ft_lerp(v0.depth, v1.depth, t.x);
	index = v0.s.y * ctx->img->width + v0.s.x;
	if (depth < ctx->z_buf[index])
	{
		ctx->z_buf[index] = depth;
		if (ctx->color_mode == AMAZING)
		{
			t.y = ft_lerp(v0.o_pos.y, v1.o_pos.y, t.x);
			t.z = ft_normalize(t.y, ctx->alt_min_max.x, ctx->alt_min_max.y);
			ctx->color = lerp_color(ctx->color1, ctx->color2, t.z);
		}
		else
			ctx->color = lerp_color(v0.color, v1.color, t.x);
		return (true);
	}
	return (false);
}
