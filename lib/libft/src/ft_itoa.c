/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:42:36 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 16:04:20 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft_utils.h"
#include "libft_str.h"

/**
 * Converts the signed integer `n` of `base` to a newly allocated string.
 * The base is validated.
 *
 * Handles negative numbers.
 *
 * @param n Signed integer to be converted.
 * @param base Base of the number.
 * @return String of the converted integer.
 */
char	*ft_itoa(int n, const char *base)
{
	char	*str;
	int		sign;
	size_t	digits;
	size_t	len;

	if (!ft_strchrdup(base))
		return (NULL);
	len = ft_strlen(base);
	sign = 1;
	if (n < 0)
		sign = -1;
	digits = ft_countdigits(n, len) + (sign == -1);
	str = malloc((digits + 1) * sizeof (char));
	if (!str)
		return (NULL);
	str[digits] = '\0';
	while (digits--)
	{
		str[digits] = base[(n * sign) % len];
		n /= len;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}
