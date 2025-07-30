/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_vector.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:30:08 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/23 19:59:47 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_VECTOR_H
# define LIBFT_VECTOR_H

# include <stdlib.h>

# include "libft_defs.h"

typedef struct s_vector
{
	void	**items;
	size_t	size;
	size_t	total;
	bool	is_heap;
}				t_vector;

bool	vector_add(t_vector *vec, void *new);
bool	vector_free(t_vector *vec);
bool	vector_del(t_vector *vec, size_t index);
void	*vector_get(t_vector *vec, size_t index);
bool	vector_init(t_vector *vec, bool is_heap);
bool	vector_set(t_vector *vec, size_t index, void *new);
int		vector_size(t_vector *vec);
int		vector_total(t_vector *vec);
void	*vector_getlast(t_vector *vec);

#endif
