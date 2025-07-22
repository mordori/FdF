/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:08:26 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/22 19:11:01 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	render_line(t_context *ctx, int i1, int i2);
static inline void	drwline(t_context *ctx, t_vertex v0, t_vertex v1, t_vec3 o);
static inline void	movepixel(t_vec2i *d, t_vec2i *s, t_vec2i *e, t_vertex *v0);
static inline void	drawpixel(t_context *ctx, t_vertex v0, uint32_t c, float z);

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
	update_ui(ctx);
	update_ui_2(ctx);
}

/**
 * Does not implement proper clipping yet.
 * Line is rejected if both vertices are outside of the frustrum,
 * or either is behind the camera.
 * Pixels on a line outside the screen are not drawn, but they are computed.
 */
static inline void	render_line(t_context *ctx, int i1, int i2)
{
	t_vertex	*v0;
	t_vertex	*v1;
	t_vec2i		visible;
	t_vec2i		s;
	bool		v0_visible;

	v0 = vector_get(ctx->verts, i1);
	v1 = vector_get(ctx->verts, i2);
	if (vert_to_screen(v0, ctx) && vert_to_screen(v1, ctx))
	{
		ctx->color = v1->color;
		s = v0->screen;
		visible.x = s.x >= 0 && s.x < (int)ctx->img->width;
		visible.y = s.y >= 0 && s.y < (int)ctx->img->height;
		v0_visible = visible.x && visible.y;
		s = v1->screen;
		visible.x = s.x >= 0 && s.x < (int)ctx->img->width;
		visible.y = s.y >= 0 && s.y < (int)ctx->img->height;
		if (v0_visible || (visible.x && visible.y))
			drwline(ctx, *v0, *v1, vec3(0.0f, v0->pos.y, v0->z));
	}
}

/**
 * Implements Bresenham's line algorithm with incremental error.
 * Also lerps the pixel color and depth from vertex data.
 * The final pixel color off by 1 step from v1 color.
 *
 * @param d
 * @param s Slopes for x and y [-1 or 1]
 * @param error
 * @param steps Steps along the line. x is current, y is total steps.
 * @param t
 */
static inline void	drwline(t_context *ctx, t_vertex v0, t_vertex v1, t_vec3 o)
{
	t_vec2i	d;
	t_vec2i	s;
	t_vec2i	error;
	t_vec2	steps;
	t_vec3	t;

	d = vec2i(abs(v1.screen.x - v0.screen.x), -abs(v1.screen.y - v0.screen.y));
	s.x = 1 + (-2 * (v0.screen.x >= v1.screen.x));
	s.y = 1 + (-2 * (v0.screen.y >= v1.screen.y));
	error.x = d.x + d.y;
	steps = vec2(0.0f, fmaxf(d.x, -d.y));
	while (v0.screen.x != v1.screen.x || v0.screen.y != v1.screen.y)
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
 * Moves the position of the pixel along the line.
 * Determines if either x or y is incremented, based on the slope of the line
 */
static inline void	movepixel(t_vec2i *d, t_vec2i *s, t_vec2i *e, t_vertex *v0)
{
	e->y = 2 * e->x;
	if (e->y >= d->y)
	{
		e->x += d->y;
		v0->screen.x += s->x;
	}
	if (e->y <= d->x)
	{
		e->x += d->x;
		v0->screen.y += s->y;
	}
}

/**
 * Implements Bresenham's line algorithm.
 * Also lerps the pixel color and depth from vertex data.
 */
static inline void	drawpixel(t_context *ctx, t_vertex v0, uint32_t c, float z)
{
	int	x;
	int	y;
	int	index;

	x = v0.screen.x;
	y = v0.screen.y;
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
