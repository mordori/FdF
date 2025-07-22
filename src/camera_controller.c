/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_controller.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:29:14 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/22 00:33:46 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	translate_rotate(t_context *ctx);
static inline void	orbit(t_context *ctx);
static inline void	pan(t_cam *cam, t_vec2i d);
static inline void	zoom(t_context *ctx);

void	control_camera(void *param)
{
	t_context		*ctx;
	t_vec2i			pos;
	t_vec2i			delta;
	static t_vec2i	prev;

	delta = vec2i(0, 0);
	ctx = param;
	if (ctx->cam.projection != ISOMETRIC)
	{
		if (ctx->spin == OFF)
			translate_rotate(ctx);
		orbit(ctx);
		zoom(ctx);
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
			mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
		{
			mlx_get_mouse_pos(ctx->mlx, &pos.x, &pos.y);
			if (prev.x >= 0 && prev.y >= 0)
				delta = vec2i_sub(pos, prev);
			pan(&ctx->cam, delta);
			update_camera(&ctx->cam);
			prev = pos;
		}
		else
			prev = vec2i_n(-1);
	}
}

static inline void	translate_rotate(t_context *ctx)
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
		ctx->transform.rot.z -= ctx->mlx->delta_time;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT))
		ctx->transform.rot.z += ctx->mlx->delta_time;
}

static inline void	orbit(t_context *ctx)
{
	static t_vec2i	prev;
	t_vec2i			pos;
	t_vec2i			d;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
		mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		mlx_get_mouse_pos(ctx->mlx, &pos.x, &pos.y);
		if (prev.x >= 0 && prev.y >= 0)
		{
			d = vec2i_sub(pos, prev);
			ctx->cam.yaw -= d.x * SENSITIVITY;
			ctx->cam.pitch += d.y * SENSITIVITY;
		}
		update_camera(&ctx->cam);
		prev = pos;
	}
	else
		prev = vec2i_n(-1);
}

static inline void	pan(t_cam *cam, t_vec2i d)
{
	float	speed;
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	speed = cam->distance * 0.0016f;
	forward = vec3_normalize(vec3_sub(cam->target, cam->eye));
	right = vec3_normalize(vec3_cross(forward, cam->up));
	up = vec3_normalize(vec3_cross(right, forward));
	cam->target = vec3_add(cam->target, vec3_scale(right, -d.x * speed));
	cam->target = vec3_add(cam->target, vec3_scale(up, d.y * speed));
}

static inline void	zoom(t_context *ctx)
{
	static t_vec2i	prev;
	t_vec2i			pos;
	float			speed;
	float			delta;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
		mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		mlx_get_mouse_pos(ctx->mlx, &pos.x, &pos.y);
		if (prev.y == 0)
			prev.y = pos.y;
		speed = ctx->cam.distance * SENSITIVITY;
		delta = pos.y - prev.y;
		ctx->cam.distance = fmaxf(0.1f, ctx->cam.distance - delta * speed);
		ctx->cam.ortho_size = ctx->cam.distance * 0.5f;
		prev.y = pos.y;
		update_camera(&ctx->cam);
	}
	else
		prev.y = 0;
}
