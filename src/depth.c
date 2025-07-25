/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   depth.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:08:22 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 15:08:55 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

bool	z_test(t_context *ctx, t_vertex v0, t_vertex v1, t_vec4 t)
{
	float	z;
	int		index;

	z = ft_lerp(v0.z, v1.z, t.x);
	index = v0.s.y * ctx->img->width + v0.s.x;
	if (z < ctx->z_buf[index])
	{
		ctx->z_buf[index] = z;
		if (ctx->color_mode == AMAZING)
		{
			t.y = ft_lerp(t.w, v1.pos.y, t.x);
			t.z = ft_normalize(t.y, ctx->alt_min_max.x, ctx->alt_min_max.y);
			ctx->color = lerp_color(ctx->color1, ctx->color2, t.z);
		}
		else
			ctx->color = lerp_color(v0.color, v1.color, t.x);
		return (true);
	}
	return (false);
}
