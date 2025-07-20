/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:18:33 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/20 06:53:04 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline t_mat4	view_matrix(t_cam cam);
static inline t_mat4	proj_persp(t_cam cam);
static inline t_mat4	proj_ortho(t_cam cam);

bool	mvp(t_vertex *vert, t_context *ctx)
{
	t_matrices	m;
	t_vec4		v_clip;
	t_vec3		v_ndc;

	m.m = model_matrix(ctx);
	m.v = view_matrix(ctx->cam);
	if (ctx->cam.projection == PERSPECTIVE)
		m.p = proj_persp(ctx->cam);
	else
		m.p = proj_ortho(ctx->cam);
	m.mvp = mat4_mul(m.p, mat4_mul(m.v, m.m));
	v_clip = mat4_mul_vec4(m.mvp, vert->pos);
	if (v_clip.w <= 0.0f)
		return (false);
	v_ndc = vec3(v_clip.x/v_clip.w, v_clip.y/v_clip.w, v_clip.z/v_clip.w);
	vert->screen.x = (v_ndc.x + 1.0f) * 0.5f * ctx->mlx->width;
	vert->screen.y = (1.0f - v_ndc.y) * 0.5f * ctx->mlx->height;
	return (true);
}

t_mat4	model_matrix(t_context *ctx)
{
	t_matrices	m;

	m.t = mat4_translate(ctx->transform.pos);
	m.r = mat4_rot(ctx->transform.rot);
	m.s = mat4_scale(ctx->transform.scale);
	m.m = mat4_mul(m.t, mat4_mul(m.r, m.s));
	return (m.m);
}

static inline t_mat4	view_matrix(t_cam cam)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	t_mat4	view;

	forward = vec3_normalize(vec3_sub(cam.target, cam.eye));
	right = vec3_normalize(vec3_cross(forward, cam.up));
	up = vec3_cross(right, forward);
	view = mat4_identity();
	view.m[0][0] = right.x;
	view.m[0][1] = right.y;
	view.m[0][2] = right.z;
	view.m[0][3] = -vec3_dot(right, cam.eye);
	view.m[1][0] = up.x;
	view.m[1][1] = up.y;
	view.m[1][2] = up.z;
	view.m[1][3] = -vec3_dot(up, cam.eye);
	view.m[2][0] = -forward.x;
	view.m[2][1] = -forward.y;
	view.m[2][2] = -forward.z;
	view.m[2][3] = vec3_dot(forward, cam.eye);
	return (view);
}

static inline t_mat4	proj_persp(t_cam cam)
{
	float	f;
	t_mat4	proj;

	f = 1.0f / tanf(cam.fov / 2.0f);
	proj = mat4_zero();
	proj.m[0][0] = f / cam.aspect;
	proj.m[1][1] = f;
	proj.m[2][2] = -(cam.far + cam.near) / (cam.near - cam.far);
	proj.m[2][3] = -(2.0f * cam.far * cam.near) / (cam.near - cam.far);
	proj.m[3][2] = -1.0f;
	return (proj);
}

static inline t_mat4	proj_ortho(t_cam cam)
{
	t_mat4	proj;
	t_vec4	sides;
	float	size;
	float	aspect;

	proj = mat4_identity();
	size = cam.ortho_size;
	aspect = cam.aspect;
	sides = vec4(-size * aspect, size * aspect, -size, size);
	proj.m[0][0] = 2.0f / (sides.y - sides.x);
	proj.m[1][1] = 2.0f / (sides.w - sides.z);
	proj.m[2][2] = -2.0f / (cam.far - cam.near);
	proj.m[0][3] = -(sides.y + sides.x) / (sides.y - sides.x);
	proj.m[1][3] = -(sides.w + sides.z) / (sides.w - sides.z);
	proj.m[2][3] = -(cam.far + cam.near) / (cam.far - cam.near);
	return (proj);
}
