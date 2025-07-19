/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:45:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/19 06:46:04 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline float	clamp_pitch(float pitch);
static inline void compute_distance(t_context *ctx);
static inline void	compute_bounds(t_context *ctx, t_vec3 *min, t_vec3 *max);

void	update_camera(t_cam *cam)
{
	t_vec3	dir;

	cam->pitch = clamp_pitch(cam->pitch);
	dir.x = cosf(cam->pitch) * sinf(cam->yaw);
	dir.y = sinf(cam->pitch);
	dir.z = cosf(cam->pitch) * cosf(cam->yaw);


	cam->eye = vec3_add(cam->target, vec3_scale(dir, cam->distance));
}

void	init_camera(t_context *ctx, t_vec3 target)
{
	ctx->cam.projection = ISOMETRIC;
	ctx->cam.target = target;
	ctx->cam.distance = 1.0f;
	ctx->cam.yaw = M_PI / 4.0f;
	ctx->cam.pitch = atanf(1.0f / sqrtf(2.0f));
	ctx->cam.up = vec3(0.0f, 1.0f, 0.0f);
	ctx->cam.fov = M_PI / 2.0f;
	ctx->cam.aspect = (float)ctx->mlx->width / ctx->mlx->height;
	ctx->cam.near = 0.1f;
	ctx->cam.far = 100.0f;
	compute_distance(ctx);
	update_camera(&ctx->cam);
}

static inline void compute_distance(t_context *ctx)
{
	float	max_dim;
	float	padding;

	padding = 1.1f;
	max_dim = fmaxf(ctx->bounds.x, ctx->bounds.y) * padding;
	if (ctx->cam.projection == PERSPECTIVE)
		ctx->cam.distance = (max_dim * 0.5f) / tanf(ctx->cam.fov * 0.5f);
	else
	{
		if (ctx->bounds.x / ctx->cam.aspect > ctx->bounds.y)
			max_dim = ctx->bounds.x;
		else
			max_dim = ctx->bounds.y;
		ctx->cam.distance = max_dim * 0.6f;
	}
}

static inline float	clamp_pitch(float pitch)
{
	const float	limit = M_PI / 2 - 0.001f;
	if (pitch > limit)
		return (limit);
	if (pitch < -limit)
		return (-limit);
	return (pitch);
}

static inline void	compute_bounds(t_context *ctx, t_vec3 *min, t_vec3 *max)
{
	t_vertex	*v;
	t_vec4		pos;
	t_mat4		transform;
	size_t		i;

	transform = mat4_translate(ctx->transform.pos);
	transform = mat4_mul(mat4_scale(ctx->transform.scale), transform);
	transform = mat4_mul(mat4_rot(ctx->transform.rot), transform);
	*min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	*max = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	i = 0;
	while (i < ctx->verts->total)
	{
		v = vector_get(ctx->verts, i++);
		pos = mat4_mul_vec4(transform, v->pos);
		min->x = fminf(min->x, pos.x);
		min->y = fminf(min->y, pos.y);
		min->z = fminf(min->z, pos.z);
		max->x = fmaxf(max->x, pos.x);
		max->y = fmaxf(max->y, pos.y);
		max->z = fmaxf(max->z, pos.z);
	}
}

void	focus(t_context *ctx)
{
	t_vec3	min;
	t_vec3	max;

	compute_bounds(ctx, &min, &max);
	ctx->center = vec3(min.x + max.x, min.y + max.y, 0.0f);
	ctx->center = vec3_scale(ctx->center, 0.5f);
	ctx->bounds = vec3_sub(max, min);
	ctx->cam.target = ctx->center;
	compute_distance(ctx);
	update_camera(&ctx->cam);
}
