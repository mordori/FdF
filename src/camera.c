/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:45:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/20 06:49:03 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void compute_distance(t_context *ctx);

void	update_camera(t_cam *cam)
{
	const float	limit = M_PI / 2 - 0.001f;
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
	max_dim = fmaxf(fmaxf(ctx->bounds.x, ctx->bounds.y), ctx->bounds.z) * padding;
	if (ctx->cam.projection == PERSPECTIVE)
		ctx->cam.distance = (max_dim * 0.5f) / tanf(ctx->cam.fov * 0.5f);
	else
	{
		ctx->cam.ortho_size = max_dim * 0.5f;
		ctx->cam.distance = max_dim;
	}
}

void	compute_bounds(t_context *ctx)
{
	t_vertex	*v;
	t_vec4		pos;
	size_t		i;
	t_mat4		m;

	m = model_matrix(ctx);
	ctx->min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	ctx->max = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	i = 0;
	while (i < ctx->verts->total)
	{
		v = vector_get(ctx->verts, i++);
		pos = mat4_mul_vec4(m, v->pos);
		ctx->min.x = fminf(ctx->min.x, pos.x);
		ctx->min.y = fminf(ctx->min.y, pos.y);
		ctx->min.z = fminf(ctx->min.z, pos.z);
		ctx->max.x = fmaxf(ctx->max.x, pos.x);
		ctx->max.y = fmaxf(ctx->max.y, pos.y);
		ctx->max.z = fmaxf(ctx->max.z, pos.z);
	}
}

void	frame(t_context *ctx)
{
	compute_bounds(ctx);
	ctx->center = vec3_scale(vec3_add(ctx->min, ctx->max), 0.5f);
	ctx->bounds = vec3_sub(ctx->max, ctx->min);
	ctx->cam.target = ctx->center;
	compute_distance(ctx);
	update_camera(&ctx->cam);
}
