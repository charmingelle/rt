/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 18:52:48 by grevenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

// int	main(int ac, char **av)
// {
	// t_env	env;

	// sgl_init();
	// init_env(&env);
	// read_scene(&env, ac, av);
	// init_scene(&env);
	// while (1)
	// {
	// 	render_scene(&env);
	// 	if (poll_events(&env) == 0)
	// 		return (sgl_quit(quit_msg, &env));
	// 	SDL_UpdateTexture(env.win->tex, NULL, env.win->surf->pixels, 1);
	// 	SDL_RenderCopy(env.win->rend, env.win->tex, NULL, NULL);
	// 	display_stats(&env, env.win->rend);
	// 	SDL_RenderPresent(env.win->rend);
	// 	MOVE_F(env.flags) ? move(&env, 1) : 0;
	// 	MOVE_B(env.flags) ? move(&env, -1) : 0;
	// }
	// return (sgl_quit(quit_msg, &env));
// }

int		main(int ac, char **av)
{
	t_env	env;

	sgl_init();
	init_env(&env);
	if (ac == 2)
	{
		read_scene(av[1], &env);
		ft_printf("env.cam->pos = (%f, %f, %f)\n", env.cam->pos.x, env.cam->pos.y, env.cam->pos.z);
		ft_printf("env.cam->rot = (%f, %f, %f)\n", env.cam->rot.x, env.cam->rot.y, env.cam->rot.z);
		ft_printf("env.cam->rot_os = %d\n", env.cam->rot_os);
		ft_printf("----------\n");		
		ft_printf("env.scene.objs_c = %d\n", env.scene.objs_c);
		ft_printf("----------\n");
		for (int i = 0; i < env.scene.objs_c; i++)
		{
			ft_printf("env.scene.objs_h[i].color = (%f, %f, %f)\n", env.scene.objs_h[i].color.x, env.scene.objs_h[i].color.y, env.scene.objs_h[i].color.z);
			ft_printf("env.scene.objs_h[i].type = %d\n", env.scene.objs_h[i].type);
			ft_printf("env.scene.objs_h[i].pos = (%f, %f, %f)\n", env.scene.objs_h[i].pos.x, env.scene.objs_h[i].pos.y, env.scene.objs_h[i].pos.z);
			ft_printf("env.scene.objs_h[i].dir = (%f, %f, %f)\n", env.scene.objs_h[i].dir.x, env.scene.objs_h[i].dir.y, env.scene.objs_h[i].dir.z);
			ft_printf("env.scene.objs_h[i].emission = (%f, %f, %f)\n", env.scene.objs_h[i].emission.x, env.scene.objs_h[i].emission.y, env.scene.objs_h[i].emission.z);
			ft_printf("env.scene.objs_h[i].rad = %f\n", env.scene.objs_h[i].rad);
			ft_printf("env.scene.objs_h[i].spec = %d\n", env.scene.objs_h[i].spec);
			ft_printf("env.scene.objs_h[i].refl = %f\n", env.scene.objs_h[i].refl);
			ft_printf("env.scene.objs_h[i].transp = %f\n", env.scene.objs_h[i].transp);
			ft_printf("env.scene.objs_h[i].text = %s\n", env.scene.objs_h[i].text);
			ft_printf("----------\n");
		}
		ft_printf("env.scene.light_c = %d\n", env.scene.light_c);
		ft_printf("----------\n");
		for (int i = 0; i < env.scene.light_c; i++)
		{
			ft_printf("env.scene.light_h[i].type = %d\n", env.scene.light_h[i].type);
			ft_printf("env.scene.light_h[i].intens = %f\n", env.scene.light_h[i].intens);
			ft_printf("eenv.scene.light_h[i].pos = (%f, %f, %f)\n", env.scene.light_h[i].pos.x, env.scene.light_h[i].pos.y, env.scene.light_h[i].pos.z);
			ft_printf("----------\n");
		}
	}
}
