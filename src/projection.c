/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:18:33 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 06:46:42 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Transforms a vertex from object space to screen space.
 *
 * The following transformation chain is applied:
 *
 * - Model Space	->	World Space		(model_matrix)
 *
 * - World Space	->	View Space		(view_matrix)
 *
 * - View Space		->	Clip Space		(projection matrix)
 *
 * - Clip Space		->	NDC Space		(divide by w)
 *
 * - NDC Space		->	Screen Space	(viewport transform)
 *
 * The result is stored in the vertex's screen coordinates `vert->s`
 * and depth `vert->z`.
 *
 * @param vert Vertex to transform.
 * @param ctx Rendering context containing the camera and transformations.
 * @return `true` if the vertex is in front of the camera, `false` otherwise.
 */
bool	project_to_screen(t_vertex *vert, t_context *ctx, t_vec4 v_clip)
{
	t_vec3	v_ndc;

	v_ndc = vec3_scale(vec3_4(v_clip), 1.0f / v_clip.w);
	vert->s.x = (v_ndc.x + 1.0f) * 0.5f * ctx->img->width;
	vert->s.y = (1.0f - v_ndc.y) * 0.5f * ctx->img->height;
	vert->z = (v_ndc.z + 1.0f) * 0.5f;
	return (true);
}

/**
 * Builds the model matrix for the current transformation by combining
 * translation, rotation, and scaling into a single matrix.
 *
 * @param ctx Rendering context containing the model's transform.
 * @return The resulting 4x4 model matrix.
 */
t_mat4	model_matrix(t_context *ctx)
{
	t_matrices	m;

	m.t = mat4_translate(ctx->transform.pos);
	m.r = mat4_rot(ctx->transform.rot);
	m.s = mat4_scale(ctx->transform.scale);
	m.m = mat4_mul(mat4_mul(m.t, m.r), m.s);
	return (m.m);
}

/**
 * Constructs the view matrix based on the camera's eye position,
 * target, and up vector.
 *
 * @param cam Camera structure containing eye, target, and up vectors.
 * @return The resulting 4x4 view matrix.
 */
t_mat4	view_matrix(t_cam cam)
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

/**
 * Constructs a perspective projection matrix using a field of view,
 * aspect ratio, and near/far planes.
 *
 * @param cam Camera structure containing fov, aspect, near and far values.
 * @return The resulting 4x4 perspective projection matrix.
 */
t_mat4	proj_persp(t_cam cam)
{
	float	f;
	t_mat4	proj;

	f = 1.0f / tanf(cam.fov / 2.0f);
	proj = mat4_zero();
	proj.m[0][0] = f / cam.aspect;
	proj.m[1][1] = f;
	proj.m[2][2] = (cam.far + cam.near) / (cam.near - cam.far);
	proj.m[2][3] = (2.0f * cam.far * cam.near) / (cam.near - cam.far);
	proj.m[3][2] = -1.0f;
	return (proj);
}

/**
 * Builds orthographic projection matrix based on the camera's ortho size,
 * aspect ratio, and near/far planes.
 *
 * Sides:
 *
 * - Left		(x)
 *
 * - Right		(y)
 *
 * - Bottom		(z)
 *
 * - Top		(w)
 *
 * @param cam Camera structure containing ortho_size, aspect, near, and far.
 * @return The resulting 4x4 orthographic projection matrix.
 */
t_mat4	proj_ortho(t_cam cam)
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
