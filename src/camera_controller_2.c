/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_controller_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:56:47 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/24 00:09:29 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	wrap_m_x(t_context *ctx, t_vec2i *pos)
{
	static int	revs_x;

	if (pos->x >= (int)ctx->img->width - 1)
	{
		mlx_set_mouse_pos(ctx->mlx, 1, pos->y);
		pos->x = 1;
		++revs_x;
	}
	else if (pos->x <= 0)
	{
		mlx_set_mouse_pos(ctx->mlx, ctx->img->width - 2, pos->y);
		pos->x = ctx->img->width - 2;
		--revs_x;
	}
	return (revs_x * (ctx->img->width - 2));
}

int	wrap_m_y(t_context *ctx, t_vec2i *pos)
{
	static int	revs_y;

	if (pos->y >= (int)ctx->img->height - 1)
	{
		mlx_set_mouse_pos(ctx->mlx, pos->x, 1);
		pos->y = 1;
		++revs_y;
	}
	else if (pos->y <= 0)
	{
		mlx_set_mouse_pos(ctx->mlx, pos->x, ctx->img->height - 2);
		pos->y = ctx->img->height - 2;
		--revs_y;
	}
	return (revs_y * (ctx->img->height - 2));
}
