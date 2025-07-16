/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:19:35 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/16 20:46:42 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	ft_error(void *param);
void compute_scale(t_render_context *ctx);
static inline void	get_bounds(t_render_context *ctx);
void	on_resize(int width, int height, void *param);
static inline uint32_t	parse_color(const char *str);
static inline uint32_t	lerp_color(uint32_t c1, uint32_t c2, float t);
int	parse_map(char *map, t_vector *vertices, t_vec2 *rows_cols, int *max_altitude);
t_vertex	*make_vertex(int x, int y, int z, uint32_t color);
void	make_indices(t_vector *indices, t_vec2 rows_cols);
t_vec3	*make_triangle(int x, int y, int z);
void	render(void *context);
static inline void	render_line(t_render_context *ctx, int i1, int i2);
static inline void	draw_line(mlx_image_t *img, t_vertex v0, t_vertex v1);
static inline void	draw_pixel(mlx_image_t *img, t_vertex v0, uint32_t color);
t_vertex	proj_iso(t_vertex *v, float scale, t_mat4 transform);
static inline float	ft_fmin(float a, float b);
static inline float	ft_fmax(float a, float b);
static inline void	clear_image(mlx_image_t *img, uint32_t color);
void	input(mlx_key_data_t keydata, void *param);
void	on_close(void* param);
t_mat4	mat4_identity(void);
t_mat4	mat4_mul(t_mat4 a, t_mat4 b);
t_mat4	mat4_rot_x(float angle);
t_mat4	mat4_rot_y(float angle);

int	main(int argc, char *argv[])
{
	mlx_t				*mlx;
	mlx_image_t			*img;
	int					fd;
	t_vector			vertices;
	t_vector			indices;
	t_vec2				rows_cols;
	t_render_context	*ctx;
	int					max_altitude;

	if (argc != 2)
		return (EXIT_FAILURE);
	mlx_set_setting(MLX_MAXIMIZED, true);
	mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
		ft_error(mlx);
	img  = mlx_new_image(mlx, mlx->width, mlx->height);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error(mlx);
	vector_init(&vertices, true);
	max_altitude = INT32_MIN;
	fd = parse_map(argv[1], &vertices, &rows_cols, &max_altitude);
	if (fd == ERROR)
		ft_error(mlx);
	vector_init(&indices, true);
	make_indices(&indices, rows_cols);
	ctx = malloc(sizeof (t_render_context));
	if (!ctx)
		ft_error(mlx);
	*ctx = (t_render_context)
	{
		.mlx = mlx,
		.img = img,
		.vertices = &vertices,
		.indices = &indices,
		.rows_cols = rows_cols,
		.project = proj_iso,
		.scale = 1.0f,
		.max_altitude = max_altitude
	};
	ctx->transform = mat4_identity();
	compute_scale(ctx);
	get_bounds(ctx);
	mlx_key_hook(mlx, input, mlx);
	mlx_loop_hook(mlx, render, ctx);
	mlx_close_hook(mlx, on_close, mlx);
	mlx_resize_hook(mlx, on_resize, ctx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	close(fd);
	vector_free(&indices);
	vector_free(&vertices);
	return (EXIT_SUCCESS);
}

t_mat4	mat4_identity(void)
{
	t_mat4	matrix;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			matrix.m[i][j] = 0.0f;
			++j;
		}
		++i;
	}
	i = 0;
	while (i < 4)
	{
		matrix.m[i][i] = 1.0f;
		++i;
	}
	return (matrix);
}

t_mat4	mat4_scale(float sx, float sy, float sz)
{
	t_mat4	matrix;

	matrix = mat4_identity();
	matrix.m[0][0] = sx;
	matrix.m[1][1] = sy;
	matrix.m[2][2] = sz;
	return (matrix);
}

t_mat4	mat4_rot_x(float angle)
{
	t_mat4	matrix;
	float	cos;
	float	sin;

	matrix = mat4_identity();
	cos = cosf(angle);
	sin = sinf(angle);
	matrix.m[1][1] = cos;
	matrix.m[1][2] = -sin;
	matrix.m[2][1] = sin;
	matrix.m[2][2] = cos;
	return (matrix);
}

t_mat4	mat4_rot_y(float angle)
{
	t_mat4	matrix;
	float	cos;
	float	sin;

	matrix = mat4_identity();
	cos = cosf(angle);
	sin = sinf(angle);
	matrix.m[0][0] = cos;
	matrix.m[0][2] = sin;
	matrix.m[2][0] = -sin;
	matrix.m[2][2] = cos;
	return (matrix);
}

t_mat4	mat4_rot_z(float angle)
{
	t_mat4	matrix;
	float	cos;
	float	sin;

	matrix = mat4_identity();
	cos = cosf(angle);
	sin = sinf(angle);
	matrix.m[0][0] = cos;
	matrix.m[0][1] = -sin;
	matrix.m[1][0] = sin;
	matrix.m[1][1] = cos;
	return (matrix);
}

t_mat4	mat4_mul(t_mat4 a, t_mat4 b)
{
	t_mat4	result;
	int		row;
	int		col;
	int		k;

	result = mat4_identity();
	row = 0;
	while(row < 4)
	{
		col = 0;
		while (col < 4)
		{
			k = 0;
			while (k < 4)
			{
				result.m[row][col] += a.m[row][k] * b.m[k][col];
				++k;
			}
			++col;
		}
		++row;
	}
	return (result);
}

t_vec3	mat4_transform_vec3(t_mat4 m, t_vec3 v)
{
	t_vec3	result;

	result.x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + m.m[0][3];
	result.y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + m.m[1][3];
	result.z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + m.m[2][3];
	return (result);
}

static inline void	clear_image(mlx_image_t *img, uint32_t color)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (i < img->width)
	{
		j = 0;
		while (j < img->height)
		{
			mlx_put_pixel(img, i, j, color);
			++j;
		}
		++i;
	}
}

void	on_close(void* param)
{
	mlx_t	*mlx;

	mlx = param;
	(void)mlx;
}

void	on_resize(int width, int height, void *param)
{
	t_render_context	*ctx;

	ctx = param;
	if (ctx->img)
		mlx_delete_image(ctx->mlx, ctx->img);
	ctx->img = mlx_new_image(ctx->mlx, width, height);
	if (!ctx->img || (mlx_image_to_window(ctx->mlx, ctx->img, 0, 0) < 0))
		ft_error(ctx->mlx);
	printf("Window resized to: %d x %d\n", width, height);
	//get_bounds(ctx);
}

static inline void ft_error(void *param)
{
	mlx_t	*mlx;

	mlx = param;
	mlx_close_window(mlx);
	perror(mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void	input(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(mlx);
}

int	parse_map(char *map, t_vector *vertices, t_vec2 *rows_cols, int *max_altitude)
{
	char		*line;
	char		**elements;
	char		**data;
	int			fd;
	int			col;
	int			row;
	t_vertex	*vertex;
	uint32_t 	color;


	fd = open(map, O_RDONLY);
	if (fd == ERROR)
		return (ERROR);
	line = get_next_line(fd);
	if (!line)
		return (ERROR);

	(*rows_cols).x = 0;
	row = 0;
	while (line)
	{
		line = ft_strtrim(line, "\n");
		elements = ft_split(line, ' ');
		if (!elements)
			return (ERROR);
		col = 0;
		while (elements[col])
		{
			data = ft_split(elements[col], ',');
			if (!data)
				return (ERROR);
			if (data[1])
				color = parse_color(data[1]);
			else
				color = (uint32_t)WHITE;
			vertex = make_vertex(col, row, ft_atoi(data[0]), color);
			vector_add(vertices, vertex);
			if (vertex->pos.z > *max_altitude)
				*max_altitude = vertex->pos.z;
			++col;
		}
		(*rows_cols).y = col;
		++row;
		(*rows_cols).x++;
		line = get_next_line(fd);
	}
	// TODO: ERROR MAP
	return (fd);
}

static inline uint32_t	parse_color(const char *str)
{
	uint32_t	color;
	char		*trimmed;
	char		padded[9];
	size_t		len;

	if (!ft_strnstr(str, "0x", 2))
	{
		// TODO: ERROR
		return (WHITE);
	}
	ft_memset(padded, '0', 8);
	padded[8] = '\0';
	trimmed = (char *)str + 2;
	trimmed = ft_strtrim(trimmed, "\n");
	ft_striteri(trimmed, ft_toupper);
	len = ft_strlen(trimmed);
	if (len >= 2 && len <= 8)
		ft_memcpy(padded, trimmed, len);
	else
	{
		// TODO: ERROR
		free(trimmed);
		return (WHITE);
	}
	color = (uint32_t)ft_atoi_base(padded, BASE_16);
	if ((color & 0xFF) == 0)
		color |= 0xFF;
	free(trimmed);
	return (color);
}

t_vertex	*make_vertex(int x, int y, int z, uint32_t color)
{
	t_vertex *vertex;

	vertex = malloc(sizeof (t_vertex));
	if (!vertex)
		return (NULL);
	vertex->pos.x = x;
	vertex->pos.y = y;
	vertex->pos.z = z;
	vertex->color = color;
	return (vertex);
}

void	make_indices(t_vector *indices, t_vec2 rows_cols)
{
	t_vec3	*index;
	t_quad	q;
	int cols;
	int row;
	int col;
	size_t	i;

	i = 0;
	while (i < (size_t)(rows_cols.x - 1) * ((rows_cols.y - 1) * 2))
	{
		cols = rows_cols.y;
		row = i / (2 * (cols - 1));
		col = (i / 2) % (cols - 1);
		q.topLeft = row * cols + col;
		q.topRight = q.topLeft + 1;
		q.bottomLeft = q.topLeft + cols;
		q.bottomRight = q.bottomLeft + 1;
		if (i % 2 == 0)
			index = make_triangle(q.topRight, q.topLeft, q.bottomLeft);
		else
			index = make_triangle(q.bottomLeft, q.bottomRight, q.topRight);
		vector_add(indices, index);
		++i;
	}
}

t_vec3	*make_triangle(int x, int y, int z)
{
	t_vec3 *vec;

	vec = malloc(sizeof (t_vec3));
	if (!vec)
		return (NULL);
	vec->x = x;
	vec->y = y;
	vec->z = z;
	return (vec);
}

void	render(void *context)
{
	size_t				i;
	t_vec3				index;
	t_render_context	*ctx;

	i = 0;
	ctx = (t_render_context *)context;
	clear_image(ctx->img, BLACK);
	while (i < ctx->indices->total)
	{
		index = *(t_vec3 *)(vector_get(ctx->indices, i));
		// TODO Hash table lines to render with no dups
		render_line(ctx, index.x, index.y);
		render_line(ctx, index.y, index.z);
		++i;
	}
}

void compute_scale(t_render_context *ctx)
{
	float proj_width;
	float proj_height;
	float scale_x;
	float scale_y;

	proj_height = (ctx->rows_cols.x + ctx->rows_cols.y) * 0.5f + ctx->max_altitude;
	proj_width = (ctx->rows_cols.x + ctx->rows_cols.y) * 1.0f;
	scale_x = ctx->img->width / proj_width;
	scale_y = ctx->img->height / proj_height;
	ctx->scale = ft_fmin(scale_x, scale_y) * 0.75;
}

static inline void	get_bounds(t_render_context *ctx)
{
	t_vec2		min;
	t_vec2		max;
	t_vertex	vertex;
	size_t		i;

	min.x = INT32_MAX;
	min.y = INT32_MAX;
	max.x = INT32_MIN;
	max.y = INT32_MIN;
	i = 0;
	while (i < ctx->vertices->total)
	{
		vertex = ctx->project(vector_get(ctx->vertices, i), ctx->scale, ctx->transform);

		if (vertex.pos.x < min.x)
			min.x = vertex.pos.x;
		if (vertex.pos.y < min.y)
			min.y = vertex.pos.y;
		if (vertex.pos.x > max.x)
			max.x = vertex.pos.x;
		if (vertex.pos.y > max.y)
			max.y = vertex.pos.y;
		++i;
	}
	ctx->offset.x = ctx->img->width / 2 - (min.x + max.x) / 2;
	ctx->offset.y = ctx->img->height / 2 - (min.y + max.y) / 2;
}

static inline void	render_line(t_render_context *ctx, int i1, int i2)
{
	t_vertex	v0;
	t_vertex	v1;

	v0 = ctx->project(vector_get(ctx->vertices, i1), ctx->scale, ctx->transform);
	v1 = ctx->project(vector_get(ctx->vertices, i2), ctx->scale, ctx->transform);
	v0.pos.x += ctx->offset.x;
	v0.pos.y += ctx->offset.y;
	v1.pos.x += ctx->offset.x;
	v1.pos.y += ctx->offset.y;
	draw_line(ctx->img, v0, v1);
}

static inline float	ft_fmax(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

static inline float	ft_fmin(float a, float b)
{
	if (a < b)
		return (a);
	return (b);
}

static inline void	draw_line(mlx_image_t *img, t_vertex v0, t_vertex v1)
{
	t_vec2	d;
	t_vec2	s;
	t_vec2	error;
	float step = 0;
	float total_steps;
	uint32_t color;

	d.x = abs(v1.pos.x - v0.pos.x);
	d.y = -abs(v1.pos.y - v0.pos.y);
	total_steps = ft_fmax(d.x, -d.y);
	s.x = 1 + (-2 * (v0.pos.x >= v1.pos.x));
	s.y = 1 + (-2 * (v0.pos.y >= v1.pos.y));
	error.x = d.x + d.y;
	while (v0.pos.x != v1.pos.x || v0.pos.y != v1.pos.y)
	{
		float t = step / total_steps;
		color = lerp_color(v0.color, v1.color, t);
		draw_pixel(img, v0, color);
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
		++step;
	}
	color = v1.color;
	draw_pixel(img, v0, color);
}

static inline uint32_t	lerp_color(uint32_t c1, uint32_t c2, float t)
{
	t_color color1;
	t_color color2;
	t_color color;

	color1.r = (c1 >> 24) & 0xFF;
	color1.g = (c1 >> 16) & 0xFF;
	color1.b = (c1 >> 8) & 0xFF;
	color1.a = c1 & 0xFF;
	color2.r = (c2 >> 24) & 0xFF;
	color2.g = (c2 >> 16) & 0xFF;
	color2.b = (c2 >> 8) & 0xFF;
	color2.a = c2 & 0xFF;
	color.r = color1.r + (int)((color2.r - color1.r) * t);
	color.g = color1.g + (int)((color2.g - color1.g) * t);
	color.b = color1.b + (int)((color2.b - color1.b) * t);
	color.a = color1.a + (int)((color2.a - color1.a) * t);
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
}

static inline void	draw_pixel(mlx_image_t *img, t_vertex v0, uint32_t color)
{
	if (v0.pos.x >= 0 && v0.pos.x < (int)img->width && \
		v0.pos.y >= 0 && v0.pos.y < (int)img->height)
		mlx_put_pixel(img, v0.pos.x, v0.pos.y, color);
}

t_vertex	proj_iso(t_vertex *v, float scale, t_mat4 transform)
{
	t_vec3	pos;

	pos = mat4_transform_vec3(transform, v->pos);
	return (t_vertex)
	{
		.pos.x = (pos.x - pos.y) * scale,
		.pos.y = (pos.x + pos.y) * scale / 2 - pos.z * scale / 2,
		.pos.z = pos.z,
		.color = v->color
	};
}
