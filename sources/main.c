/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/10 16:38:23 by grevenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

// int	main(int ac, char **av)
// {
// 	t_env	env;

// 	sgl_init();
// 	init_env(&env);
// 	read_scene(&env, ac, av);
// 	render_scene(&env);
// 	while (1)
// 	{
// 		if (poll_events(&env) == 0)
// 			return (sgl_quit());
// 		SDL_UpdateTexture(env.win->tex, NULL, env.win->surf->pixels, 1);
// 		SDL_RenderCopy(env.win->rend, env.win->tex, NULL, NULL);
// 		display_fps(env.win->rend);
// 		SDL_RenderPresent(env.win->rend);
// 		CAMERA_M(env.flags) ? render_scene(&env) : 0;
// 		CAMERA_M(env.flags) ? camera_rotate(&env) : 0;
// 		sgl_vsync();
// 	}
// 	return (sgl_quit());
// }

int		main(int ac, char **av)
{
	t_env	env;

	sgl_init();
	init_env(&env);
	if (ac == 2)
	{
		// env.cam->pos.x = 25;
		read_scene(av[1], &env);
		// ft_printf("env = %p\n", &env);
		// ft_printf("cam = %p\n", env.cam);
		// ft_printf("pos = %p\n", &env.cam->pos);
		// ft_printf("%p - %f\n", &env.cam->pos.x, env.cam->pos.x);
		ft_printf("env.cam->pos = (%f, %f, %f)\n", env.cam->pos.x, env.cam->pos.y, env.cam->pos.z);
		ft_printf("env.cam->rot = (%f, %f, %f)\n", env.cam->rot.rx, env.cam->rot.ry, env.cam->rot.rz);
		ft_printf("env.cam->rot_os = %d\n", env.cam->rot_os);		
		// ft_printf("%f\n", env.cam->rot_os);
	}
}
