/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 14:01:53 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/02 21:21:37 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	key_handler(SDL_Scancode key, t_env *env)
{
	t_uchar	e;

	e = 0;
	key == SDL_SCANCODE_P ? env->flags ^= 0b1 : 0;
	key == SDL_SCANCODE_KP_PLUS && ++e ? env->cam->vwp->dist += 0.1 : 0;
	key == SDL_SCANCODE_KP_MINUS && ++e ? env->cam->vwp->dist -= 0.1 : 0;
	e && !CAMERA_M(env->flags) ? render_scene(env) : 0;
}

int			poll_events(t_env *env)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
		if (e.window.event == SDL_WINDOWEVENT_CLOSE
			|| (e.type == SDL_KEYDOWN && e.key.keysym.sym == EXIT_KEY))
			return (0);
		else if (e.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			sgl_win_resize(e.window.windowID, e.window.data1, e.window.data2);
			cl_reinit_mem(&env->cl, &env->cam->kl.mem,
							e.window.data1 * e.window.data2 * 4, 0);
			resize_viewport(env->cam->vwp, e.window.data1, e.window.data2);
			render_scene(env);
		}
		else if (e.type == SDL_KEYDOWN)
			key_handler(e.key.keysym.scancode, env);
	return (1);
}
