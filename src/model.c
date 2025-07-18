/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:07:51 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/19 07:21:33 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	init_context(t_context *ctx, mlx_t *mlx, mlx_image_t *img);
static inline void normalize_model(t_context *ctx);

int	initialize(char *file, t_context **ctx, mlx_t *mlx, mlx_image_t *img)
{
	t_vector	*verts;
	t_vector	*tris;
	t_vec2		rows_cols;

	verts = malloc(sizeof (t_vector));
	tris = malloc(sizeof (t_vector));
	*ctx = malloc(sizeof (t_context));
	verts->items = NULL;
	tris->items = NULL;
	if (!verts || !tris || !*ctx)
		return (fdf_free(verts, tris, *ctx), ERROR);
	if (!vector_init(verts, true))
		return (fdf_free(verts, tris, *ctx), ERROR);
	if (parse_map(file, verts, &rows_cols) == ERROR)
		return (fdf_free(verts, tris, *ctx), ERROR);
	if (!vector_init(tris, true))
		return (fdf_free(verts, tris, *ctx), ERROR);
	if(!make_triangles(tris, rows_cols))
		return (fdf_free(verts, tris, *ctx), ERROR);
	(*ctx)->verts = verts;
	(*ctx)->tris = tris;
	(*ctx)->rows_cols = rows_cols;
	init_context(*ctx, mlx, img);
	return (true);
}

static inline void	init_context(t_context *ctx, mlx_t *mlx, mlx_image_t *img)
{
	normalize_model(ctx);
	ctx->transform.pos = vec3_scale(ctx->center, -1.0f);
	ctx->transform.rot = vec3(0.0f, 0.0f, 0.0f);
	ctx->transform.scale = vec3_f(1.0f);
	ctx->mlx = mlx;
	ctx->img = img;
	init_camera(ctx, vec3(0, 0, 0));
}

static inline void normalize_model(t_context *ctx)
{
	t_vec3		min;
	t_vec3		max;
	t_vec3		bounds;
	t_vertex	*v;
	size_t		i;

	min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	max = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	i = 0;
	while (i < ctx->verts->total)
	{
		v = vector_get(ctx->verts, i++);
		min.x = fminf(min.x, v->pos.x);
		min.y = fminf(min.y, v->pos.y);
		min.z = fminf(min.z, v->pos.z);
		max.x = fmaxf(max.x, v->pos.x);
		max.y = fmaxf(max.y, v->pos.y);
		max.z = fmaxf(max.z, v->pos.z);
	}
	ctx->center = vec3_scale(vec3(min.x + max.x, min.y + max.y, 0.0f), 0.5f);
	bounds = vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	ctx->bounds = bounds;
}
