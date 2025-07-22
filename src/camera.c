/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:45:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/22 11:23:35 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	compute_distance(t_context *ctx);

/**
 * Initializes and updates the camera with computed distance.
 *
 * @param ctx Model context.
 */
void	frame(t_context *ctx)
{
	t_vertex	v;

	ctx->cam.aspect = (float)ctx->mlx->width / ctx->mlx->height;
	compute_bounds(ctx, WORLD, 0, &v);
	ctx->cam.target = ctx->center;
	compute_distance(ctx);
	update_camera(&ctx->cam);
}

/**
 * Initializes and updates the camera with computed distance.
 *
 * @param cam Camera.
 */
void	update_camera(t_cam *cam)
{
	const float	limit = M_PI_2 - 0.001f;
	t_vec3	dir;

	if (cam->pitch > limit)
		cam->pitch = limit;
	if (cam->pitch < -limit)
		cam->pitch = -limit;
	dir.x = cosf(cam->pitch) * sinf(cam->yaw);
	dir.y = sinf(cam->pitch);
	dir.z = cosf(cam->pitch) * cosf(cam->yaw);
	cam->eye = vec3_add(cam->target, vec3_scale(dir, cam->distance));
}

/**
 * Initializes and updates the camera with computed distance.
 *
 * @param ctx Model context.
 */
void	init_camera(t_context *ctx)
{
	ctx->cam.projection = ISOMETRIC;
	ctx->cam.aspect = (float)ctx->mlx->width / ctx->mlx->height;
	ctx->cam.target = vec3_n(0.0f);
	ctx->cam.up = vec3(0.0f, 1.0f, 0.0f);
	ctx->cam.yaw = M_PI / 4.0f;
	ctx->cam.pitch = atanf(1.0f / sqrtf(2.0f));
	ctx->cam.fov = M_PI / 2.5f;
	ctx->cam.near = 0.1f;
	ctx->cam.far = 100.0f;
	compute_distance(ctx);
	update_camera(&ctx->cam);
}

/**
 * Initializes and updates the camera with computed distance.
 *
 * @param ctx Model context.
 */
static inline void	compute_distance(t_context *ctx)
{
	float	max_dim;
	float	padding;

	padding = 1.1f;
	max_dim = fmaxf(fmaxf(ctx->bounds.x, ctx->bounds.y), ctx->bounds.z);
	max_dim *= padding;
	if (ctx->cam.projection == PERSPECTIVE)
		ctx->cam.distance = (max_dim * 0.5f) / tanf(ctx->cam.fov * 0.5f);
	else
	{
		ctx->cam.ortho_size = max_dim * 0.5f;
		ctx->cam.distance = max_dim;
	}
}
