/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 20:32:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 12:27:46 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		quit_msg(t_env *env)
{
	cl_float3	*colors;
	int			it;
	int			fd;

	ft_printf("Rendering done!\n");
	if ((fd = open("backup.ppm", O_WRONLY | O_TRUNC | O_CREAT,
								S_IWUSR | S_IRUSR)) != -1)
	{
		dprintf(fd, "P3\n%d %d\n%d\n", env->win->w, env->win->h, 255);
		it = -1;
		colors = env->scene.colors_h;
		while (++it < env->win->w * env->win->h)
			dprintf(fd, "%d %d %d ", TORGB(colors[it].x), TORGB(colors[it].y),
				TORGB(colors[it].z));
		ft_printf("{green}Image successfully saved to 'backup.ppm'{nc}\n");
	}
	ft_printf("{blue}Good bye!{nc}\n");
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
