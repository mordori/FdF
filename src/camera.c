/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:45:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/29 13:31:52 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Positions and updates the camera to frame the entire model within view.
 *
 * - Computes the model bounds in WORLD space.
 *
 * - Updates the camera's aspect ratio based on the current window size.
 *
 * - Sets the camera's target to the model's center.
 *
 * - Computes camera distance based on model bounds.
 *
 * - Calls `update_camera()` to apply pitch, yaw, and distance.
 *
 * @param ctx Rendering context containing the model and camera.
 */
void	frame(t_context *ctx)
{
	t_vertex	v;

	ctx->cam.aspect = (float)ctx->img->width / ctx->img->height;
	ctx->m.m = model_matrix(ctx);
	compute_bounds(ctx, WORLD, 0, &v);
	ctx->cam.target = ctx->center;
	compute_distance(ctx);
	update_camera(&ctx->cam);
}

/**
 * Updates the camera's eye based on its yaw, pitch, distance, and target.
 *
 * Constraints the pitch angle within [-π/2, π/2] to avoid gimbal lock.
 * The camera's `eye` position is calculated relative to `target`
 * using spherical coordinates defined by yaw, pitch, and distance.
 *
 * @param cam Camera.
 */
void	update_camera(t_cam *cam)
{
	const float	limit = M_PI_2 - 0.001f;
	t_vec3		dir;

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
 * Initializes the camera with default values, then computes the camera distance
 * based on the model's bounds and updates the camera's `eye` position.
 *
 * @param ctx Rendering context containing model bounds and camera.
 */
void	init_camera(t_context *ctx)
{
	ctx->cam.projection = ISOMETRIC;
	ctx->cam.aspect = (float)ctx->img->width / ctx->img->height;
	ctx->cam.target = vec3_n(0.0f);
	ctx->cam.up = vec3(0.0f, 1.0f, 0.0f);
	ctx->cam.yaw = M_PI / 4.0f;
	ctx->cam.pitch = M_PI / 5.1f;
	ctx->cam.fov = M_PI / 2.5f;
	ctx->cam.near = 0.1f;
	ctx->cam.panning = false;
	ctx->cam.zooming = false;
	ctx->cam.orbiting = false;
	compute_distance(ctx);
	ctx->cam.far = 8.0f * ctx->cam.distance;
	update_camera(&ctx->cam);
}

/**
 * Computes the appropriate camera distance or orthographic size
 * needed to fit the model within the view frustum. Adjusted with aspect ratio.
 *
 * Padding factor (1.1x) is applied.
 *
 * @param ctx Rendering context containing the camera and model bounds.
 */
void	compute_distance(t_context *ctx)
{
	const float	padding = 1.5f;
	float		max_dim;

	max_dim = fmaxf(fmaxf(ctx->bounds.x, ctx->bounds.y), ctx->bounds.z);
	if (ctx->img->width < ctx->img->height)
		max_dim /= ctx->cam.aspect;
	max_dim *= padding;
	if (ctx->cam.projection == PERSPECTIVE)
		ctx->cam.distance = (max_dim * 0.5f) / tanf(ctx->cam.fov * 0.5f);
	else
	{
		ctx->cam.ortho_size = max_dim * 0.5f;
		ctx->cam.distance = max_dim;
	}
}
