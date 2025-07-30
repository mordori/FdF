/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:56:47 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/31 01:51:18 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Wraps the mouse horizontally across the screen boundaries.
 *
 * If the mouse reaches the right or left edge of the window, it is
 * repositioned to the opposite side. The number of wraps is tracked internally.
 *
 * @param ctx Rendering context containing mlx context and render image.
 * @param pos Current mouse screen position.
 * @return The total horizontal displacement: wraps × (window width - 2)
 */
int	wrap_m_x(t_context *ctx, t_vec2i *pos)
{
	static int	wraps;

	if (pos->x >= (int)ctx->img->width - 1)
	{
		mlx_set_mouse_pos(ctx->mlx, 1, pos->y);
		pos->x = 1;
		++wraps;
	}
	else if (pos->x <= 0)
	{
		mlx_set_mouse_pos(ctx->mlx, ctx->img->width - 2, pos->y);
		pos->x = ctx->img->width - 2;
		--wraps;
	}
	return (wraps * (ctx->img->width - 2));
}

/**
 * Wraps the mouse vertically across the screen boundaries.
 *
 * If the mouse reaches the top or bottom edge of the window, it is
 * repositioned to the opposite side. The number of wraps is tracked internally.
 *
 * @param ctx Rendering context containing mlx context and render image.
 * @param pos Current mouse screen position.
 * @return The total vertical displacement: wraps × (window height - 2).
 */
int	wrap_m_y(t_context *ctx, t_vec2i *pos)
{
	static int	wraps;

	if (pos->y >= (int)ctx->img->height - 1)
	{
		mlx_set_mouse_pos(ctx->mlx, pos->x, 1);
		pos->y = 1;
		++wraps;
	}
	else if (pos->y <= 0)
	{
		mlx_set_mouse_pos(ctx->mlx, pos->x, ctx->img->height - 2);
		pos->y = ctx->img->height - 2;
		--wraps;
	}
	return (wraps * (ctx->img->height - 2));
}

/**
 * Translates and rotates the model based on keyboard input.
 *
 * Movement and rotation speeds are frame-rate independent.
 *
 * @param ctx Rendering context containing model transform and mlx context.
 */
void	translate_rotate(t_context *ctx)
{
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_W))
		ctx->transform.pos.z -= ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_S))
		ctx->transform.pos.z += ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_A))
		ctx->transform.pos.x -= ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_D))
		ctx->transform.pos.x += ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_UP))
		ctx->transform.rot.x -= ctx->mlx->delta_time;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN))
		ctx->transform.rot.x += ctx->mlx->delta_time;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT))
		ctx->transform.rot.z += ctx->mlx->delta_time;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT))
		ctx->transform.rot.z -= ctx->mlx->delta_time;
}

/**
 * Loop hook for discrete input commands.
 *
 * - [ESC]		exit program.
 *
 * - [P]		switch projection.
 *
 * - [C]		toggle color mode.
 *
 * - [F]		frame the model.
 *
 * - [SPACE]	toggle spin mode.
 *
 * - [R]		reset model transform and camera angle.
 *
 * @param keydata Mlx key data.
 * @param param Rendering context.
 */
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
	if (keydata.key == MLX_KEY_P && keydata.action == MLX_RELEASE)
	{
		ctx->cam.projection = (ctx->cam.projection + 1) % 3;
		if (ctx->cam.projection == ISOMETRIC)
		{
			ctx->color_mode = DEFAULT;
			reset_transforms(ctx);
		}
	}
	if (ctx->cam.projection == ISOMETRIC)
		return ;
	if (keydata.key == MLX_KEY_F && keydata.action == MLX_RELEASE)
		frame(ctx);
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_RELEASE)
		ctx->spin_mode = !ctx->spin_mode;
	if (keydata.key == MLX_KEY_R && keydata.action == MLX_RELEASE)
		reset_transforms(ctx);
	if (keydata.key == MLX_KEY_C && keydata.action == MLX_RELEASE)
		ctx->color_mode = !ctx->color_mode;
}
/**
 * Adjusts the camera's field of view (FOV) in perspective projection mode.
 *
 * - [U]	decrease FOV.
 *
 * - [I]	increase FOV.
 *
 * Frame-rate independent. The result is clamped between 30 degrees and
 * 135 degrees. The camera distance is recomputed after a change.
 *
 * @param ctx Rendering context containing camera and mlx context.
 */
void	control_fov(t_context *ctx)
{
	if (ctx->cam.projection == PERSPECTIVE)
	{
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_U))
		{
			ctx->cam.fov -= ctx->mlx->delta_time;
			ctx->cam.fov = ft_clamp(ctx->cam.fov, M_PI / 6.0f, 3 * M_PI / 4.0f);
			compute_distance(ctx);
		}
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_I))
		{
			ctx->cam.fov += ctx->mlx->delta_time;
			ctx->cam.fov = ft_clamp(ctx->cam.fov, M_PI / 6.0f, 3 * M_PI / 4.0f);
			compute_distance(ctx);
		}
	}
}
