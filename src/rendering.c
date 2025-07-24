/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:08:26 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/24 12:26:58 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	render_line(t_context *ctx, int idx0, int idx1);
static inline void	drwline(t_context *ctx, t_vertex v0, t_vertex v1, t_vec3 o);
static inline void	movepixel(t_vec2i *d, t_vec2i *s, int *error, t_vertex *v0);
static inline void	drawpixel(t_context *ctx, t_vertex v0, uint32_t c, float z);

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
	v_rc = vec2i(ctx->rows_cols.x - 1, ctx->rows_cols.y - 1);
	i = 0;
	while (i < ctx->tris->total)
	{
		if (i % 2 == 0 || \
(i / (2 * v_rc.y) == (size_t)v_rc.x - 1 && i % 2) || \
((i / 2) % v_rc.y == (size_t)v_rc.y - 1 && i % 2))
		{
			index = vector_get(ctx->tris, i);
			render_line(ctx, index->x, index->y);
			render_line(ctx, index->y, index->z);
		}
		++i;
	}
}

/**
 * Does not implement proper clipping yet.
 *
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
	t_vertex	*v0;
	t_vertex	*v1;
	t_vec2i		visible;
	t_vec2i		s;
	bool		v0_visible;

	v0 = vector_get(ctx->verts, idx0);
	v1 = vector_get(ctx->verts, idx1);
	if (vert_to_screen(v0, ctx) && vert_to_screen(v1, ctx))
	{
		ctx->color = v0->color;
		s = v0->s;
		visible.x = s.x >= 0 && s.x < (int)ctx->img->width;
		visible.y = s.y >= 0 && s.y < (int)ctx->img->height;
		v0_visible = visible.x && visible.y;
		s = v1->s;
		visible.x = s.x >= 0 && s.x < (int)ctx->img->width;
		visible.y = s.y >= 0 && s.y < (int)ctx->img->height;
		if (v0_visible || (visible.x && visible.y))
			drwline(ctx, *v0, *v1, vec3(0.0f, v0->pos.y, v0->z));
	}
}

/**
 * Implements Bresenham's line algorithm with incremental error tracking
 * as best approximation to the ideal line.
 * Interpolates both color and depth (z) along the line.
 *
 * The final pixel color on a line is not exact to the target vertex color.
 *
 * @param ctx Rendering context containing colors and altitude range.
 * @param v0 Starting vertex (screen pos, color, depth)
 * @param v1 Ending vertex (screen pos, color, depth)
 * @param o Auxiliary vector for depth and altitude interpolation.
 */
static inline void	drwline(t_context *ctx, t_vertex v0, t_vertex v1, t_vec3 o)
{
	t_vec2i	d;
	t_vec2i	s;
	int		error;
	t_vec2	steps;
	t_vec3	t;

	d = vec2i(abs(v1.s.x - v0.s.x), abs(v1.s.y - v0.s.y));
	s = vec2i(1 + (-2 * (v0.s.x >= v1.s.x)), 1 + (-2 * (v0.s.y >= v1.s.y)));
	error = d.x - d.y;
	steps = vec2(0.0f, fmaxf(d.x, d.y));
	while (v0.s.x != v1.s.x || v0.s.y != v1.s.y)
	{
		t.x = steps.x++ / steps.y;
		ctx->color = lerp_color(v0.color, v1.color, t.x);
		o.z = ft_lerp(v0.z, v1.z, t.x);
		if (ctx->colors == AMAZING)
		{
			t.y = ft_lerp(o.y, v1.pos.y, t.x);
			t.z = ft_normalize(t.y, ctx->alt_min_max.x, ctx->alt_min_max.y);
			ctx->color = lerp_color(ctx->color1, ctx->color2, t.z);
		}
		drawpixel(ctx, v0, ctx->color, o.z);
		movepixel(&d, &s, &error, &v0);
	}
	drawpixel(ctx, v0, ctx->color, o.z);
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
static inline void	movepixel(t_vec2i *d, t_vec2i *s, int *error, t_vertex *v0)
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
static inline void	drawpixel(t_context *ctx, t_vertex v0, uint32_t c, float z)
{
	int	x;
	int	y;
	int	index;

	x = v0.s.x;
	y = v0.s.y;
	if (x >= 0 && x < (int)ctx->img->width && \
y >= 0 && y < (int)ctx->img->height)
	{
		index = y * ctx->img->width + x;
		if (z < ctx->z_buf[index])
		{
			ctx->z_buf[index] = z;
			mlx_put_pixel(ctx->img, x, y, c);
		}
	}
}
