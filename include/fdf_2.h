/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_2.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:49:10 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/22 19:09:00 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_2_H
# define FDF_2_H

# include "fdf.h"

void	compute_bounds(t_context *ctx, t_space space, size_t i, t_vertex *v);
void	initialize(char *file, t_context **ctx, mlx_t *mlx, mlx_image_t *img);

#endif
