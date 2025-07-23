/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:04:16 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/23 14:53:34 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline int			parse_data(char **elem, t_vector *verts, int row);
static inline int			init_line(char *line, char ***elements);
static inline uint32_t	parse_color(const char *str);
static inline void		free_parse(char *line, char **elements);

/**
 * Parses a map file into a vector of vertices.
 *
 * The map file must have matching width and height values as dimensions.
 *
 * Reads a heightmap file line by line, parsing each line into vertex data.
 * Each vertex can optionally have a color.
 * The resulting vertices are stored in `verts`,
 * and the number of rows and columns is stored in `rows_cols`.
 *
 * @param map Path to the map file.
 * @param verts Vector where vertices will be stored.
 * @param rows_cols Structure to store rows (x) and columns (y).
 * @return `true` on success, or ERROR on failure.
 */
int	parse_map(char *map, t_vector *verts, t_vec2i *rows_cols)
{
	char	*line;
	char	**elements;
	int		fd;
	int		col;

	fd = open(map, O_RDONLY);
	if (fd == ERROR)
		return (ERROR);
	line = get_next_line(fd);
	if (!line)
		return (ERROR);
	rows_cols->x = 0;
	while (line)
	{
		if (init_line(line, &elements) == ERROR)
			return (free_parse(line, elements), ERROR);
		col = parse_data(elements, verts, rows_cols->x);
		if (col == ERROR || (rows_cols->x && col != rows_cols->y))
			return (free_parse(line, elements), ERROR);
		rows_cols->x++;
		rows_cols->y = col;
		free_parse(line, elements);
		line = get_next_line(fd);
	}
	return (close(fd), true);
}

/**
 * Prepares a line for parsing by trimming and splitting.
 *
 * Removes the trailing newline character, and splits the line into
 * elements separated by spaces.
 *
 * @param line The input line from the map file.
 * @param elements Output pointer to an array of split string elements.
 * @return true on success, or ERROR on allocation failure.
 */
static inline int	init_line(char *line, char ***elements)
{
	size_t	len;

	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		line[len - 1] = '\0';
	*elements = ft_split(line, ' ');
	if (!*elements)
		return (ERROR);
	return (true);
}

/**
 * Frees resources allocated during parsing.
 *
 * @param line Current line string.
 * @param elements Array of strings.
 */
static inline void	free_parse(char *line, char **elements)
{
	if (elements)
		ft_free_split(elements);
	free(line);
}

/**
 * Parses a row of map elements into vertices.
 *
 * For each element in the row, this function splits the string into a height
 * and optional color. A new vertex is created and added to the vertex vector.
 *
 * @param elem Array of strings representing height and optional color.
 * @param verts Vector where parsed vertices are added.
 * @param row Current row index, used as (y) for the vertex.
 * @return Number of columns parsed, or ERROR on failure.
 */
static inline int	parse_data(char **elem, t_vector *verts, int row)
{
	char		**data;
	uint32_t	color;
	int			col;
	t_vertex	*vert;

	col = 0;
	while (elem[col])
	{
		color = WHITE;
		data = ft_split(elem[col], ',');
		if (!data)
			return (ERROR);
		if (data[1] && !ft_strnstr(data[1], "0x", 2))
			return (ft_free_split(data), ERROR);
		if (data[1])
			color = parse_color(data[1]);
		vert = make_vert(col++, row, ft_atoi(data[0]), color);
		if (!vert)
			return (ft_free_split(data), ERROR);
		vector_add(verts, vert);
		ft_free_split(data);
	}
	return (col);
}

/**
 * Converts a color string in hex format to a 32-bit RGBA value.
 *
 * @param str The color string in case-insensitive hex format.
 * @return 32-bit RGBA color, or ERROR_COLOR on failure.
 */
static inline uint32_t	parse_color(const char *str)
{
	uint32_t	color;
	char		*trimmed;
	char		padded[9];
	size_t		len;

	ft_memset(padded, '0', 8);
	padded[8] = '\0';
	trimmed = (char *)str + 2;
	trimmed = ft_strtrim(trimmed, "\n");
	ft_striteri(trimmed, ft_toupper);
	len = ft_strlen(trimmed);
	if (len >= 2 && len <= 8)
		ft_memcpy(padded, trimmed, len);
	else
		return (free(trimmed), ERROR_COLOR);
	color = ft_atouint32_t_base(padded, BASE_16);
	if ((color & 0xFF) == 0)
		color |= 0xFF;
	return (free(trimmed), color);
}
