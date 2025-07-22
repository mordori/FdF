/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:07:51 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/22 19:10:46 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "fdf_2.h"

static inline void	init_context(t_context *ctx, mlx_t *mlx, mlx_image_t *img);
static inline void	apply_corrections(t_context *ctx);

/**
 * Begins parsing the map file, initializes verts and tris vector arrays,
 * fills tris and beings initialing the model context.
 *
 * @param ctx Pointer to the model context.
 * @param mlx Mlx context.
 * @param img Render image.
 */
void	initialize(char *file, t_context **ctx, mlx_t *mlx, mlx_image_t *img)
{
	t_vector	*verts;
	t_vector	*tris;
	t_vec2i		rows_cols;

	verts = malloc(sizeof (t_vector));
	tris = malloc(sizeof (t_vector));
	verts->items = NULL;
	tris->items = NULL;
	if (!verts || !tris)
		return (fdf_free(verts, tris, NULL), ft_error(mlx, "verts/tris alloc"));
	if (!vector_init(verts, true))
		return (fdf_free(verts, tris, NULL), ft_error(mlx, "verts init"));
	if (parse_map(file, verts, &rows_cols) == ERROR)
		return (fdf_free(verts, tris, NULL), ft_error(mlx, "map parse"));
	if (!vector_init(tris, true))
		return (fdf_free(verts, tris, NULL), ft_error(mlx, "tris init"));
	if (!make_triangles(tris, rows_cols))
		return (fdf_free(verts, tris, NULL), ft_error(mlx, "tris fill"));
	*ctx = malloc(sizeof (t_context));
	if (!*ctx)
		return (fdf_free(verts, tris, *ctx), ft_error(mlx, "ctx alloc"));
	(*ctx)->verts = verts;
	(*ctx)->tris = tris;
	(*ctx)->rows_cols = rows_cols;
	init_context(*ctx, mlx, img);
}

/**
 * Computes the min/max altitudes, the center and the bounds of the model,
 * and applies corrective translation and rotation to the vertices.
 *
 * @param ctx Model context.
 * @param mlx Mlx context.
 * @param img Render image.
 */
void	compute_bounds(t_context *ctx, t_space space, size_t i, t_vertex *v)
{
	t_vec4		pos;
	t_vec3		min;
	t_vec3		max;

	min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	max = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	while (i < ctx->verts->total)
	{
		v = vector_get(ctx->verts, i++);
		pos = v->pos;
		if (space == WORLD)
			pos = mat4_mul_vec4(model_matrix(ctx), v->pos);
		min.x = fminf(min.x, pos.x);
		min.y = fminf(min.y, pos.y);
		min.z = fminf(min.z, pos.z);
		max.x = fmaxf(max.x, pos.x);
		max.y = fmaxf(max.y, pos.y);
		max.z = fmaxf(max.z, pos.z);
		if (space == OBJECT)
			ctx->alt_min_max.x = ft_imin(ctx->alt_min_max.x, pos.z);
		if (space == OBJECT)
			ctx->alt_min_max.y = ft_imax(ctx->alt_min_max.y, pos.z);
	}
	ctx->center = vec3_scale(vec3_add(min, max), 0.5f);
	ctx->bounds = vec3_sub(max, min);
}

/**
 * Initializes the model context. Begins normalization of the model,
 * and initialization of the camera.
 *
 * @param ctx Model context.
 * @param mlx Mlx context.
 * @param img Render image.
 */
static inline void	init_context(t_context *ctx, mlx_t *mlx, mlx_image_t *img)
{
	size_t		i;
	t_vertex	v;

	ctx->z_buf = malloc(sizeof(float) * img->width * img->height);
	if (!ctx->z_buf)
		return (fdf_free(ctx->verts, ctx->tris, ctx), ft_error(mlx, "z alloc"));
	i = 0;
	while (i < img->width * img->height)
		ctx->z_buf[i++] = INFINITY;
	ctx->mlx = mlx;
	ctx->img = img;
	ctx->transform.pos = vec3_n(0.0f);
	ctx->transform.rot = vec3_n(0.0f);
	ctx->transform.scale = vec3_n(1.0f);
	ctx->alt_min_max = vec2i(INT_MAX, INT_MIN);
	ctx->colors = DEFAULT;
	ctx->color = WHITE;
	ctx->time_rot = 0.0;
	ctx->spin = OFF;
	compute_bounds(ctx, OBJECT, 0, &v);
	if (ctx->alt_min_max.x == ctx->alt_min_max.y)
		ctx->alt_min_max.y = ctx->alt_min_max.x + 1;
	apply_corrections(ctx);
	compute_bounds(ctx, WORLD, 0, &v);
	init_camera(ctx);
}

/**
 * Computes the min/max altitudes, the center and the bounds of the model,
 * and applies corrective translation and rotation to the vertices.
 *
 * @param ctx Model context.
 * @param mlx Mlx context.
 * @param img Render image.
 */
static inline void	apply_corrections(t_context *ctx)
{
	t_vertex	*v;
	size_t		i;

	i = 0;
	while (i < ctx->verts->total)
	{
		v = vector_get(ctx->verts, i++);
		v->pos = vec4_sub(v->pos, vec4_3(ctx->center, 0.0f));
		v->pos = mat4_mul_vec4(mat4_rot_x(-M_PI_2), v->pos);
	}
}

/**
 * Resets model transform, camera pitch and yaw, turns spinning off,
 * and frames the model.
 *
 * @param ctx Model context.
 */
void	reset_transforms(t_context *ctx)
{
	ctx->spin = OFF;
	ctx->transform.pos = vec3_n(0.0f);
	ctx->transform.rot = vec3_n(0.0f);
	ctx->transform.scale = vec3_n(1.0f);
	ctx->cam.yaw = M_PI / 4.0f;
	ctx->cam.pitch = atanf(1.0f / sqrtf(2.0f));
	//ctx->cam.pitch = 0;
	//ctx->cam.yaw = 0;
	//ctx->cam.fov = 0.01f;
	ctx->time_rot = 0.0;
	frame(ctx);
}
