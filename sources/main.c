/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/17 17:12:44 by grevenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		main(int ac, char **av)
{
	t_env	env;

	sgl_init();
	init_env(&env);
	read_scene(av[1], ac, &env);
	init_scene(&env);
	while (true)
	{
		render_scene(&env);
		if (poll_events(&env) == 0)
			return (sgl_quit(quit_msg, &env));
		SDL_UpdateTexture(env.win->tex, NULL, env.win->surf->pixels, 1);
		SDL_RenderCopy(env.win->rend, env.win->tex, NULL, NULL);
		display_stats(&env, env.win->rend);
		SDL_RenderPresent(env.win->rend);
		handle_movement(&env);
	}
	return (sgl_quit(quit_msg, &env));
}
