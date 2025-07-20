/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:08:26 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/21 00:15:05 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	renderline(t_context *ctx, int i1, int i2);
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
			renderline(ctx, index->x, index->y);
			renderline(ctx, index->y, index->z);
		}
		++i;
	}
	update_ui(ctx);
	update_ui_2(ctx);
}

static inline void	renderline(t_context *ctx, int i1, int i2)
{
	t_vertex	*v0;
	t_vertex	*v1;
	bool		x_visible;
	bool		y_visible;
	bool		v0_visible;

	v0 = vector_get(ctx->verts, i1);
	v1 = vector_get(ctx->verts, i2);
	if (mvp(v0, ctx) && mvp(v1, ctx))
	{
		x_visible = v0->screen.x >= 0 && v0->screen.x < (int)ctx->img->width;
		y_visible = v0->screen.y >= 0 && v0->screen.y < (int)ctx->img->height;
		v0_visible = x_visible && y_visible;
		x_visible = v1->screen.x >= 0 && v1->screen.x < (int)ctx->img->width;
		y_visible = v1->screen.y >= 0 && v1->screen.y < (int)ctx->img->height;
		if (v0_visible || (x_visible && y_visible))
			drwline(ctx, *v0, *v1, vec3(0.0f, v0->pos.y, v0->z));
	}
}

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
		if(z < ctx->z_buf[index])
		{
			ctx->z_buf[index] = z;
			mlx_put_pixel(ctx->img, x, y, c);
		}
	}
}
