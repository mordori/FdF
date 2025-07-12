/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:19:35 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/12 04:03:46 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	ft_error(void *param);
int	parse_map(char *map, t_vector *vertices, t_vec2 *rows_cols);
void	make_indices(t_vector *indices, t_vector *vertices, t_vec2 rows_cols);
void	render(void *context);
static inline void	render_line(t_render_context *ctx, int i1, int i2);
static inline void	draw_line(mlx_image_t *img, t_vertex v0, t_vertex v1);
static inline void	draw_pixel(mlx_image_t *img, t_vertex v0);
t_vertex	proj_iso(t_vertex *v);

int	main(int argc, char *argv[])
{
	mlx_t				*mlx;
	mlx_image_t			*img;
	int					fd;
	t_vector			vertices;
	t_vector			indices;
	t_vec2				rows_cols;
	t_render_context	*ctx;

	for (int fd = 3; fd < 1024; fd++)
		close(fd);

	if (argc != 2)
		return (EXIT_FAILURE);
	mlx_set_setting(MLX_MAXIMIZED, true);
	mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
		ft_error(mlx);
	img  = mlx_new_image(mlx, mlx->width, mlx->height);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error(mlx);

	mlx_image_t *imag = mlx_put_string(mlx, "Reading map file...", mlx->width / 2, mlx->height / 3);
	vector_init(&vertices, true);
	fd = parse_map(argv[1], &vertices, &rows_cols);
	if (fd == ERROR)
		ft_error(mlx);
	else
		mlx_delete_image(mlx, imag);
	vector_init(&indices, true);
	make_indices(&indices, &vertices, rows_cols);
	ctx = malloc(sizeof (t_render_context));
	if (!ctx)
		ft_error(mlx);
	*ctx = (t_render_context)
	{
		.img = img,
		.vertices = &vertices,
		.indices = &indices,
		.rows_cols = rows_cols,
		.project = proj_iso
	};
	mlx_loop_hook(mlx, render, ctx);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	close(fd);
	vector_free(&indices);
	vector_free(&vertices);
	return (EXIT_SUCCESS);
}

static inline void ft_error(void *param)
{
	mlx_t	*mlx;

	mlx = param;
	mlx_close_window(mlx);
	perror(mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

int	parse_map(char *map, t_vector *vertices, t_vec2 *rows_cols)
{
	char		*line;
	char		**elements;
	char		**data;
	int			fd;
	int			i;
	t_vertex	vertex;

	fd = open(map, O_RDONLY);
	if (fd == ERROR)
		return (ERROR);
	line = get_next_line(fd);
	if (!line)
		return (ERROR);
	(*rows_cols).x = 0;
	while (line)
	{
		elements = ft_split(line, ' ');
		if (!elements)
			return (ERROR);
		i = 0;
		while (elements[i])
		{
			data = ft_split(elements[i], ',');
			if (!data)
				return (ERROR);
			vertex.pos.z = ft_atoi(data[0]);
			if (data[1])
			{
				char *color = ft_strtrim(data[1], "0x");
				ft_striteri(color, ft_toupper);
				vertex.color = (uint32_t)ft_atoi_base(color, BASE_16);
			}
			else
				vertex.color = WHITE;
			vector_add(vertices, &vertex);
			++i;
		}
		(*rows_cols).y = i;
		(*rows_cols).x++;
		line = get_next_line(fd);
	}
	// ERROR MAP
	// strcmp
	return (fd);
}

void	make_indices(t_vector *indices, t_vector *vertices, t_vec2 rows_cols)
{
	size_t	i;
	t_vec3	index;

	i = 0;
	while (i < (rows_cols.x - 1) * ((rows_cols.y - 1) * 2))
	{
		index.x = i + i / (rows_cols.y - 1) - (i / 2) / (rows_cols.y - 1);
		index.y = rows_cols.y + i % (rows_cols.y - 1) + i / (rows_cols.y - 1);
		index.z = (i + 1) % (rows_cols.y) + i / (rows_cols.y - 1);
		vector_add(indices, &index);
		printf("%d %d %d\n", index.x, index.y, index.z);
		++i;
	}
}

void	render(void *context)
{
	size_t				i;
	t_vec3				index;
	t_render_context	*ctx;

	i = 0;
	ctx = (t_render_context *)context;
	while (i < ctx->indices->total)
	{
		index = *(t_vec3 *)(vector_get(ctx->indices, i));
		if (index.z > 0)
			render_line(ctx, index.x, index.z);
		if (index.y > 0)
			render_line(ctx, index.x, index.y);
		// if (index->y > 0 && index->z > 0)
		// 	render_line(ctx, (*index).y, (*index).z);
		++i;
	}
}

static inline void	render_line(t_render_context *ctx, int i1, int i2)
{
	t_vertex	v0;
	t_vertex	v1;
	t_vec2		offset;

	offset.x = ctx->img->width / 2;
	offset.y = 50;
	v0 = ctx->project(vector_get(ctx->vertices, i1));
	v0.pos.x += offset.x;
	v0.pos.y += offset.y;
	v1 = ctx->project(vector_get(ctx->vertices, i2));
	v1.pos.x += offset.x;
	v1.pos.y += offset.y;
	draw_line(ctx->img, v0, v1);
}

static inline void	draw_line(mlx_image_t *img, t_vertex v0, t_vertex v1)
{
	t_vec2	d;
	t_vec2	s;
	t_vec2	error;

	d.x = abs(v1.pos.x - v0.pos.x);
	d.y = -abs(v1.pos.y - v0.pos.y);
	s.x = 1 + (-2 * (v0.pos.x >= v1.pos.x));
	s.y = 1 + (-2 * (v0.pos.y >= v1.pos.y));
	error.x = d.x + d.y;
	while (v0.pos.x != v1.pos.x || v0.pos.y != v1.pos.y)
	{
		draw_pixel(img, v0);
		error.y = 2 * error.x;
		if (error.y >= d.y)
		{
			error.x += d.y;
			v0.pos.x += s.x;
		}
		if (error.y <= d.x)
		{
			error.x += d.x;
			v0.pos.y += s.y;
		}
	}
	draw_pixel(img, v0);
}

static inline void	draw_pixel(mlx_image_t *img, t_vertex v0)
{
	if (v0.pos.x >= 0 && v0.pos.x < (int)img->width && \
		v0.pos.y >= 0 && v0.pos.y < (int)img->height)
		mlx_put_pixel(img, v0.pos.x, v0.pos.y, v0.color);
}

t_vertex	proj_iso(t_vertex *v)
{
	return (t_vertex)
	{
		.pos.x = ((*v).pos.x - (*v).pos.y) * TILE_WIDTH / 2,
		.pos.y = ((*v).pos.x + (*v).pos.y) * TILE_HEIGHT / 2 - (*v).pos.z * ALTITUDE_SCALE,
		.pos.z = (*v).pos.z,
		.color = (*v).color
	};
}
