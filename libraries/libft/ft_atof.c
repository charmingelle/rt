/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 20:27:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/05 21:30:36 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_atof(const char *src)
{
	char		*fract;
	long long	tmp;
	int			sign;
	double		ret;

	sign = 1;
	if ((ret = ft_atoll(src)) < 0)
		sign = -1;
	if (!(fract = ft_strchr(src, '.')))
		return (ret);
	tmp = ft_atoll(fract + 1);
	ret += (double)tmp / ft_pow(10, ft_nbrlen(tmp)) * (double)sign;
	return (ret);
}
