/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:08:26 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 19:51:28 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	render_line(
						t_context *ctx, int idx0, int idx1);
static inline void	draw_line(
						t_context *ctx, t_vertex v0, t_vertex v1);
static inline void	move_pixel(
						t_vec2i *d, t_vec2i *s, int *error, t_vertex *v0);
static inline void	draw_pixel(
						t_context *ctx, t_vertex v0, uint32_t c);

/**
 * Renders the wireframe grid from the triangle list.
 *
 * Clears the render image first to a solid color and default the Z-buffer.
 *
 * Then, for every tringle in the mesh:
 *
 * - For every second triangle, top and left edges are drawn.
 *
 * - For tringles in the last row or last column, edges as drawn for both
 * triangles to ensure the boundary lines are rendered.
 *
 * @param param Rendering context.
 */
void	render(void *param)
{
	size_t		i;
	t_vec3		*index;
	t_context	*ctx;
	t_vec2i		v_rc;

	ctx = param;
	clear_image(ctx, 0xFF000000);
	ctx->m.m = model_matrix(ctx);
	ctx->m.v = view_matrix(ctx->cam);
	ctx->m.p = proj_ortho(ctx->cam);
	if (ctx->cam.projection == PERSPECTIVE)
		ctx->m.p = proj_persp(ctx->cam);
	ctx->m.mvp = mat4_mul(mat4_mul(ctx->m.p, ctx->m.v), ctx->m.m);
	v_rc = vec2i(ctx->rows_cols.x - 1, ctx->rows_cols.y - 1);
	i = -1;
	while (++i < ctx->tris->total)
	{
		if (i % 2 == 0 || (i / (2 * v_rc.y) == (size_t)v_rc.x - 1 && i % 2) || \
((i / 2) % v_rc.y == (size_t)v_rc.y - 1 && i % 2))
		{
			index = vector_get(ctx->tris, i);
			render_line(ctx, index->x, index->y);
			render_line(ctx, index->y, index->z);
		}
	}
}

/**
 * Line is rejected if both vertices are outside of the viewport,
 * or either is behind the camera.
 *
 * Pixels on a line outside the screen are not drawn, but they are computed.
 *
 * @param ctx Rendering context containing vertices, render image, and color.
 * @param idx0 Index for vertex 0.
 * @param idx1 Index for vertex 1.
 */
static inline void	render_line(t_context *ctx, int idx0, int idx1)
{
	t_vertex	v0;
	t_vertex	v1;
	t_vec4		s_clip;

	v0 = *(t_vertex *)vector_get(ctx->verts, idx0);
	v1 = *(t_vertex *)vector_get(ctx->verts, idx1);
	v0.o_pos = v0.pos;
	v1.o_pos =  v1.pos;
	v0.pos = mat4_mul_vec4(ctx->m.mvp, v0.pos);
	v1.pos = mat4_mul_vec4(ctx->m.mvp, v1.pos);
	if (!liang_barsky_clip(&v0, &v1))
		return ;
	project_to_screen(&v0, ctx);
	project_to_screen(&v1, ctx);
	s_clip = vec4(v0.s.x, v0.s.y, v1.s.x, v1.s.y);
	if (!liang_barsky_screen(ctx, &s_clip))
		return ;
	v0.s = vec2i_f(s_clip.x, s_clip.y);
	v1.s = vec2i_f(s_clip.z, s_clip.w);
	ctx->color = v0.color;
	draw_line(ctx, v0, v1);
}

/**
 * Implements Bresenham's line algorithm with incremental error tracking
 * as best approximation to the ideal line.
 * Interpolates both color and depth (z) along the line.
 *
 * @param ctx Rendering context containing colors and altitude range.
 * @param v0 Starting vertex (screen pos, color, depth)
 * @param v1 Ending vertex (screen pos, color, depth)
 * @param o Auxiliary vector for depth and altitude interpolation.
 */
static inline void	draw_line(
						t_context *ctx, t_vertex v0, t_vertex v1)
{
	t_vec2i	d;
	t_vec2i	s;
	int		error;
	t_vec2i	steps;
	t_vec3	t;

	d = vec2i(abs(v1.s.x - v0.s.x), abs(v1.s.y - v0.s.y));
	s = vec2i(1 + (-2 * (v0.s.x >= v1.s.x)), 1 + (-2 * (v0.s.y >= v1.s.y)));
	error = d.x - d.y;
	steps = vec2i(0.0f, ft_imax(d.x, d.y));
	while (steps.x <= steps.y)
	{
		t.x = (float)steps.x++ / steps.y;
		if (depth_test(ctx, v0, v1, t))
			draw_pixel(ctx, v0, ctx->color);
		move_pixel(&d, &s, &error, &v0);
	}
}

/**
 * Advances the current pixel position along the line.
 * Uses the accumulated error term to determine a step in
 * x, y, or both directions.
 *
 * @param d Absolute difference in x and y between endpoints.
 * @param s Step direction for x and y (-1 or +1).
 * @param error Accumulated error term, updated when stepping.
 * @param v0 Current vertex, screen-space coordinates updated in-place.
 */
static inline void	move_pixel(
						t_vec2i *d, t_vec2i *s, int *error, t_vertex *v0)
{
	t_vec2i	*delta;
	t_vec2i	*steps;
	t_vec2i	*screen;
	int		e2;

	delta = d;
	steps = s;
	screen = &(v0->s);
	e2 = 2 * (*error);
	if (e2 > -delta->y)
	{
		*error -= delta->y;
		screen->x += steps->x;
	}
	if (e2 < delta->x)
	{
		*error += delta->x;
		screen->y += steps->y;
	}
}

/**
 * Draws a pixel if it is within the render image boundaries and
 * and passes the depth test against the Z-buffer. Updates Z-buffer when drawn.
 *
 * @param ctx Rendering context containing render image and Z-buffer.
 * @param v0 Current vertex with interpolated screen-space coordinates (x, y).
 * @param c Pixel color (32-bit RGBA).
 * @param z Interpolated depth value for the pixel, used for depth testing.
 */
static inline void	draw_pixel(
						t_context *ctx, t_vertex v0, uint32_t c)
{
	int			x;
	int			y;
	uint32_t	*pixels;
	uint32_t	abgr_c;

	x = v0.s.x;
	y = v0.s.y;
	pixels = (uint32_t *)ctx->img->pixels;
	if (x >= 0 && x < (int)ctx->img->width && \
y >= 0 && y < (int)ctx->img->height)
	{
		abgr_c =	(c & 0xFF) << 24 |
					(c & 0xFF00) << 8 |
					(c & 0xFF0000) >> 8 |
					(c & 0xFF000000) >> 24;
		pixels[y * ctx->img->width + x] = abgr_c;
	}
}
