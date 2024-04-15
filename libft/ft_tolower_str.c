/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 11:55:51 by mminet            #+#    #+#             */
/*   Updated: 2024/04/15 12:18:59 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_tolower_str(char *str)
{
	int		i;
	char	*tmp;

	tmp = ft_strdup(str);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] >= 'A' && tmp[i] <= 'Z')
			tmp[i] = tmp[i] + 32;
		i++;
	}
	return (tmp);
}
