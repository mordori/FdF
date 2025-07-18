/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:18:33 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/18 07:25:23 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline t_mat4	view_matrix(t_view view);
t_mat4	proj_persp(float fov, float aspect, float near, float far);

bool	mvp(t_vertex *vert, t_context *ctx)
{
	t_matrices	m;
	t_vec4		v_clip;
	t_vec3		v_ndc;

	m.t = mat4_translate(ctx->transform.pos);
	m.r = mat4_rot_x(ctx->transform.rot.x);
	m.r = mat4_mul(mat4_rot_y(ctx->transform.rot.y), m.r);
	m.r = mat4_mul(mat4_rot_z(ctx->transform.rot.z), m.r);
	m.s = mat4_scale(ctx->transform.scale);
	m.m = mat4_mul(m.t, mat4_mul(m.r, m.s));
	m.v = view_matrix(ctx->view);
	m.p = proj_persp(ctx->fov, ctx->aspect, ctx->near, ctx->far);
	m.mvp = mat4_mul(m.p, mat4_mul(m.v, m.m));
	v_clip = mat4_mul_vec4(m.mvp, vert->pos);
	if (v_clip.w <= 0.0f)
		return (false);
	v_ndc.x = v_clip.x / v_clip.w;
	v_ndc.y = v_clip.y / v_clip.w;
	v_ndc.z = v_clip.z / v_clip.w;
	vert->screen.x = (v_ndc.x + 1.0f) * 0.5f * ctx->mlx->width;
	vert->screen.y = (1.0f - v_ndc.y) * 0.5f * ctx->mlx->height;
	return (true);
}

static inline t_mat4	view_matrix(t_view view)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	t_mat4	matrix;

	//forward = vec3_normalize(vec3_sub(view.target, view.eye));
	forward = view.target;
	right = vec3_normalize(vec3_cross(forward, view.up));
	up = vec3_cross(right, forward);
	matrix = mat4_identity();
	matrix.m[0][0] = right.x;
	matrix.m[0][1] = right.y;
	matrix.m[0][2] = right.z;
	matrix.m[0][3] = -vec3_dot(right, view.eye);
	matrix.m[1][0] = up.x;
	matrix.m[1][1] = up.y;
	matrix.m[1][2] = up.z;
	matrix.m[1][3] = -vec3_dot(up, view.eye);
	matrix.m[2][0] = -forward.x;
	matrix.m[2][1] = -forward.y;
	matrix.m[2][2] = -forward.z;
	matrix.m[2][3] = vec3_dot(forward, view.eye);
	return (matrix);
}

t_mat4	proj_persp(float fov, float aspect, float near, float far)
{
	float	f;
	t_mat4	matrix;

	f = 1.0f / tanf(fov / 2.0f);
	matrix = mat4_zero();
	matrix.m[0][0] = f / aspect;
	matrix.m[1][1] = f;
	matrix.m[2][2] = (far + near) / (near - far);
	matrix.m[2][3] = (2.0f * far * near) / (near - far);
	matrix.m[3][2] = -1.0f;
	return (matrix);
}

// t_vertex	proj_iso(t_vertex *v, float scale, t_mat4 transform)
// {
// 	t_vec3	pos;

// 	pos = v->pos;
// 	(void)transform;
// 	//pos = mat4_transform_vec3(transform, v->pos);
// 	return (t_vertex)
// 	{
// 		.pos.x = (pos.x - pos.y) * scale,
// 		.pos.y = (pos.x + pos.y) * scale / 2 - pos.z * scale / 2,
// 		.pos.z = pos.z,
// 		.color = v->color
// 	};
// }

// void compute_scale(t_context *ctx)
// {
// 	float proj_width;
// 	float proj_height;
// 	float scale_x;
// 	float scale_y;

// 	proj_height = (ctx->rows_cols.x + ctx->rows_cols.y) * 0.5f + ctx->max_alt;
// 	proj_width = (ctx->rows_cols.x + ctx->rows_cols.y) * 1.0f;
// 	scale_x = ctx->img->width / proj_width;
// 	scale_y = ctx->img->height / proj_height;
// 	ctx->scale = fminf(scale_x, scale_y) * 0.75;
// }

// void	get_bounds(t_context *ctx)
// {
// 	t_vec2		min;
// 	t_vec2		max;
// 	t_vertex	v;
// 	size_t		i;

// 	min.x = INT32_MAX;
// 	min.y = INT32_MAX;
// 	max.x = INT32_MIN;
// 	max.y = INT32_MIN;
// 	i = 0;
// 	while (i < ctx->verts->total)
// 	{
// 		v = ctx->project(vector_get(ctx->verts, i), ctx->scale, ctx->transform);
// 		if (v.pos.x < min.x)
// 			min.x = v.pos.x;
// 		if (v.pos.y < min.y)
// 			min.y = v.pos.y;
// 		if (v.pos.x > max.x)
// 			max.x = v.pos.x;
// 		if (v.pos.y > max.y)
// 			max.y = v.pos.y;
// 		++i;
// 	}
// 	ctx->offset.x = ctx->img->width / 2 - (min.x + max.x) / 2;
// 	ctx->offset.y = ctx->img->height / 2 - (min.y + max.y) / 2;
// }
