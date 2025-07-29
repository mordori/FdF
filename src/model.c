/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:07:51 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/29 21:42:29 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	init_context(t_context *ctx, mlx_t *mlx, mlx_image_t *img);
static inline void	normalize_model(t_context *ctx);

/**
 * Parses the map file, initializes the vertex and triangle vectors,
 * constructs the triangle list, and allocates the main rendering context.
 *
 * On failure, frees allocated resources and reports an error via `ft_error()`.
 *
 * @param file Path to the map file to load.
 * @param ctx Rendering context.
 * @param mlx Mlx context.
 * @param img Render image.
 */
void	initialize(char *file, t_context **ctx, mlx_t *mlx, mlx_image_t *img)
{
	t_vector	*verts;
	t_vector	*tris;
	t_vec2i		rows_cols;

	verts = malloc(sizeof (t_vector));
	if (!verts)
		return (ft_error(mlx, "verts alloc", NULL));
	tris = malloc(sizeof (t_vector));
	if (!tris)
		return (free(verts), \
ft_error(mlx, "tris alloc", NULL));
	if (!vector_init(verts, true))
		return (fdf_free(verts, NULL, NULL), free(tris), \
ft_error(mlx, "verts init", NULL));
	if (parse_map(file, verts, &rows_cols) == ERROR || rows_cols.x < 2)
		return (fdf_free(verts, NULL, NULL), free(tris), \
ft_error(mlx, "parse map", NULL));
	if (!vector_init(tris, true))
		return (fdf_free(verts, tris, NULL), \
ft_error(mlx, "tris init", NULL));
	if (!make_triangles(tris, rows_cols))
		return (fdf_free(verts, tris, NULL), \
ft_error(mlx, "make triangles", NULL));
	*ctx = malloc(sizeof (t_context));
	if (!*ctx)
		return (fdf_free(verts, tris, NULL), \
ft_error(mlx, "ctx alloc", NULL));
	(*ctx)->verts = verts;
	(*ctx)->tris = tris;
	(*ctx)->rows_cols = rows_cols;
	init_context(*ctx, mlx, img);
}

/**
 * Computes the axis-aligned bounding box of the model by
 * finding the minimum/maximum x, y, and z coordinates among all vertices.
 *
 * @param ctx Rendering context.
 * @param space Coordinate space in which bounds are computed (OBJECT or WORLD).
 * @param i Auxiliary starting index in the vertex array (always 0).
 * @param v Auxiliary vertex pointer.
 */
void	compute_bounds(t_context *ctx, t_space space, size_t i, t_vertex *v)
{
	t_vec4	pos;
	t_vec3	min;
	t_vec3	max;

	min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	max = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	while (i < ctx->verts->total)
	{
		v = vector_get(ctx->verts, i++);
		pos = v->pos;
		if (space == WORLD)
			pos = mat4_mul_vec4(ctx->m.m, v->pos);
		min.x = fminf(min.x, pos.x);
		min.y = fminf(min.y, pos.y);
		min.z = fminf(min.z, pos.z);
		max.x = fmaxf(max.x, pos.x);
		max.y = fmaxf(max.y, pos.y);
		max.z = fmaxf(max.z, pos.z);
		if (space != OBJECT)
			continue ;
		ctx->alt_min_max.x = ft_imin(ctx->alt_min_max.x, pos.z);
		ctx->alt_min_max.y = ft_imax(ctx->alt_min_max.y, pos.z);
	}
	ctx->center = vec3_scale(vec3_add(min, max), 0.5f);
	ctx->bounds = vec3_sub(max, min);
}

/**
 * Initializes the rendering context after loading vertices and triangles.
 *
 * - Allocates and clears the Z-buffer.
 *
 * - Initializes default transform values (position, rotation, scale).
 *
 * - Computes initial object-space bounds and altitudes.
 *
 * - Normalizes the model (centers and rotates it).
 *
 * - Computes world-space bounds after normalization.
 *
 * - Initializes the camera.
 *
 * @param ctx Rendering context.
 * @param mlx Mlx context.
 * @param img Render image.
 */
static inline void	init_context(t_context *ctx, mlx_t *mlx, mlx_image_t *img)
{
	static size_t	i;
	t_vertex		v;

	ctx->z_buf = malloc(sizeof(float) * img->width * img->height);
	if (!ctx->z_buf)
		return (fdf_free(ctx->verts, ctx->tris, ctx), \
ft_error(mlx, "z-buf alloc", ctx));
	while (i < img->width * img->height)
		ctx->z_buf[i++] = INFINITY;
	ctx->mlx = mlx;
	ctx->img = img;
	ctx->transform.pos = vec3_n(0.0f);
	ctx->transform.rot = vec3_n(0.0f);
	ctx->transform.scale = vec3_n(1.0f);
	ctx->alt_min_max = vec2i(INT_MAX, INT_MIN);
	ctx->color_mode = DEFAULT;
	ctx->color = WHITE;
	ctx->time_rot = 0.0;
	ctx->spin_mode = OFF;
	compute_bounds(ctx, OBJECT, 0, &v);
	if (ctx->alt_min_max.x == ctx->alt_min_max.y)
		ctx->alt_min_max.y = ctx->alt_min_max.x + 1;
	normalize_model(ctx);
	ctx->m.m = model_matrix(ctx);
	compute_bounds(ctx, WORLD, 0, &v);
	init_camera(ctx);
}

/**
 * Normalizes the model by translating all vertices, placing the center
 * of the model at the world origin, and rotating it around the
 * X-axis by -90 degrees to align the model with the coordinate system.
 *
 * @param ctx Rendering context containing the model vertices.
 */
static inline void	normalize_model(t_context *ctx)
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
 * Resets the model and camera to their default transform state:
 *
 * - Disables spin.
 *
 * - Resets model translation, rotation, and scale to identity.
 *
 * - Resets camera yaw and pitch to default viewing angles.
 *
 * - Calls `frame()` to adjust the camera to fit the model in the render image.
 *
 * @param ctx Rendering context.
 */
void	reset_transforms(t_context *ctx)
{
	ctx->spin_mode = OFF;
	ctx->transform.pos = vec3_n(0.0f);
	ctx->transform.rot = vec3_n(0.0f);
	ctx->transform.scale = vec3_n(1.0f);
	ctx->cam.yaw = M_PI / 4.0f;
	ctx->cam.pitch = atanf(1.0f / sqrtf(2.0f));
	ctx->time_rot = 0.0;
	ctx->cam.fov = M_PI / 2.5f;
	frame(ctx);
}
