/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:08:26 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/20 02:54:59 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	render_line(t_context *ctx, int i1, int i2);
static inline void	draw_line(t_context *ctx, t_vertex v0, t_vertex v1);
static inline void	move_pixel(t_vec2i *d, t_vec2i *s, t_vec2i *e, t_vertex *v0);
static inline void	draw_pixel(t_context *ctx, t_vertex v0, uint32_t c);

void	render(void *param)
{
	size_t		i;
	t_vec3		*index;
	t_context	*ctx;

	i = 0;
	ctx = param;
	clear_image(ctx->img, 0xFF000000);
	while (i < ctx->tris->total)
	{
		index = vector_get(ctx->tris, i);
		// TODO Hash table lines to render with no dups
		render_line(ctx, index->x, index->y);
		render_line(ctx, index->y, index->z);
		++i;
	}
	update_ui(ctx);
	update_ui_2(ctx);
}

static inline void	render_line(t_context *ctx, int i1, int i2)
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
			draw_line(ctx, *v0, *v1);
	}
}

static inline void	draw_line(t_context *ctx, t_vertex v0, t_vertex v1)
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
		v0.color = lerp_color(v0.color, v1.color, t.x);
		if (ctx->colors == AMAZING)
		{
			t.y = ft_lerp(v0.pos.y, v1.pos.y, t.x);
			t.z = ft_normalize(t.y, ctx->alt_min_max.x, ctx->alt_min_max.y);
			v0.color = lerp_color(ctx->color1, ctx->color2, t.z);
		}
		draw_pixel(ctx, v0, v0.color);
		move_pixel(&d, &s, &error, &v0);
	}
	draw_pixel(ctx, v0, v0.color);
}

static inline void	move_pixel(t_vec2i *d, t_vec2i *s, t_vec2i *e, t_vertex *v0)
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

static inline void	draw_pixel(t_context *ctx, t_vertex v0, uint32_t c)
{
	if (v0.screen.x >= 0 && v0.screen.x < (int)ctx->img->width && \
		v0.screen.y >= 0 && v0.screen.y < (int)ctx->img->height)
		mlx_put_pixel(ctx->img, v0.screen.x, v0.screen.y, c);
}
