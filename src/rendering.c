/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:08:26 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/19 06:54:18 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	render_line(t_context *ctx, int i1, int i2);
static inline void	draw_line(mlx_image_t *img, t_vertex v0, t_vertex v1);
static inline void	move_pixel(t_vec2 *d, t_vec2 *s, t_vec2 *e, t_vertex *v0);
static inline void	draw_pixel(mlx_image_t *img, t_vertex v0, uint32_t c);

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
			draw_line(ctx->img, *v0, *v1);
	}
}

static inline void	draw_line(mlx_image_t *img, t_vertex v0, t_vertex v1)
{
	t_vec2	d;
	t_vec2	s;
	t_vec2	error;
	t_vec2	steps;
	float	t;

	d = vec2(abs(v1.screen.x - v0.screen.x), -abs(v1.screen.y - v0.screen.y));
	s.x = 1 + (-2 * (v0.screen.x >= v1.screen.x));
	s.y = 1 + (-2 * (v0.screen.y >= v1.screen.y));
	error.x = d.x + d.y;
	steps = vec2(0, fmaxf(d.x, -d.y));
	while (v0.screen.x != v1.screen.x || v0.screen.y != v1.screen.y)
	{
		t = (float)steps.x++ / steps.y;
		draw_pixel(img, v0, lerp_color(v0.color, v1.color, t));
		move_pixel(&d, &s, &error, &v0);
	}
	draw_pixel(img, v0, v1.color);
}

static inline void	move_pixel(t_vec2 *d, t_vec2 *s, t_vec2 *e, t_vertex *v0)
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

static inline void	draw_pixel(mlx_image_t *img, t_vertex v0, uint32_t c)
{
	if (v0.screen.x >= 0 && v0.screen.x < (int)img->width && \
		v0.screen.y >= 0 && v0.screen.y < (int)img->height)
		mlx_put_pixel(img, v0.screen.x, v0.screen.y, c);
}
