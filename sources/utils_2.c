/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 20:32:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/18 15:46:27 by grevenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		screenshot(t_env *env, char *name)
{
	char		*str;
	long		tmp;
	cl_float3	*colors;
	int			it;
	int			fd;

	str = malloc(12 * env->win->w * env->win->h + 128);
	if ((fd = open(name, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR)) != -1)
	{
		tmp = sprintf(str, "P3\n%d %d\n%d\n", env->win->w, env->win->h, 255);
		it = -1;
		colors = env->scene.colors_h;
		while (++it < env->win->w * env->win->h)
			tmp += sprintf(str + tmp, "%d %d %d ",
				TORGB(colors[it].x), TORGB(colors[it].y), TORGB(colors[it].z));
		write(fd, str, ft_strlen(str));
		ft_printf("{green}Screenshot successfully saved to '%s'{nc}\n", name);
	}
	free(str);
}

void		quit_msg(t_env *env)
{
	screenshot(env, "backup.ppm");
	ft_printf("{red}Rendering done!{nc}\n");
}

inline int	count_splited(char **arr)
{
	int	count;

	count = 0;
	while (arr[count])
		count++;
	return (count);
}

inline void	free_splited(char **arr)
{
	int		i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

// float		vlen(cl_float3 a)
// {
// 	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
// }

// cl_float3	vmult(cl_float3 a, float num)
// {
// 	cl_float3	res;

// 	res.x = num * a.x;
// 	res.y = num * a.y;
// 	res.z = num * a.z;
// 	return (res);
// }

// cl_float3	normalize(cl_float3 v)
// {
// 	float	len;
// 	float	inv_len;

// 	len = vlen(v);
// 	if (len > 0)
// 	{
// 		inv_len = 1.0 / len;
// 		return (vmult(v, inv_len));
// 	}
// 	return (v);
// }
