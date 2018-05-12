/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 14:01:53 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 14:12:19 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	key_down_handler(SDL_Scancode key, t_env *env)
{
	t_uchar	e;

	e = 0;
	key == SDL_SCANCODE_M ? env->flags ^= 0b1 : 0;
	SDL_SetRelativeMouseMode(INTERACTIVE_M(env->flags));
	INTERACTIVE_M(env->flags) ? handle_movement(key, env, 1) : 0;
	e ? init_scene(env) : 0;
}

static void	key_up_handler(SDL_Scancode key, t_env *env)
{
	t_uchar	e;

	e = 0;
	INTERACTIVE_M(env->flags) ? handle_movement(key, env, 0) : 0;
	e ? init_scene(env) : 0;
}

static void	mouse_move_handler(t_env *env)
{
	int		x;
	int		y;

	SDL_GetRelativeMouseState(&x, &y);
	if (ABS(x) > 60 || ABS(y) > 60)
		return ;
	env->cam->rot.x -= (float)y * 0.3F;
	env->cam->rot.y -= (float)x * 0.3F;
	init_scene(env);
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
			resize_viewport(env->cam->vwp, e.window.data1, e.window.data2);
			init_scene(env);
		}
		else if (e.type == SDL_KEYDOWN)
			key_down_handler(e.key.keysym.scancode, env);
		else if (e.type == SDL_KEYUP)
			key_up_handler(e.key.keysym.scancode, env);
		else if (e.type == SDL_MOUSEMOTION && INTERACTIVE_M(env->flags))
			mouse_move_handler(env);
	return (1);
}
