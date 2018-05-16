/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/12 11:52:31 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/15 17:09:15 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	move(t_env *env, char state, t_point dir)
{
	const float	speed = 0.2F + 0.3F * env->flags.sprint;
	cl_float3	d;

	d = tofloat3(sgl_rotate_point(dir,
		(t_rotate){env->cam->rot.x, env->cam->rot.y, env->cam->rot.z}));
	env->cam->pos = state > 0 ? vec_add(env->cam->pos, vec_mult_num(d, speed))
							: vec_sub(env->cam->pos, vec_mult_num(d, speed));
	init_scene(env);
}

void	movement_keys(SDL_Scancode key, t_env *env, char state)
{
	key == SDL_SCANCODE_W ? env->flags.move_f = state : 0;
	key == SDL_SCANCODE_S ? env->flags.move_b = state : 0;
	key == SDL_SCANCODE_A ? env->flags.sidle_l = state : 0;
	key == SDL_SCANCODE_D ? env->flags.sidle_r = state : 0;
	key == SDL_SCANCODE_Q ? env->flags.rotate_l = state : 0;
	key == SDL_SCANCODE_E ? env->flags.rotate_r = state : 0;
	key == SDL_SCANCODE_SPACE ? env->flags.move_u = state : 0;
	key == SDL_SCANCODE_LCTRL ? env->flags.move_d = state : 0;
	key == SDL_SCANCODE_LSHIFT ? env->flags.sprint = state : 0;
}

void	handle_movement(t_env *env)
{
	t_uchar	e;

	e = 0;
	env->flags.move_f ? move(env, 1, (t_point){0, 0, 1}) : 0;
	env->flags.move_b ? move(env, -1, (t_point){0, 0, 1}) : 0;
	env->flags.sidle_l ? move(env, -1, (t_point){1, 0, 0}) : 0;
	env->flags.sidle_r ? move(env, 1, (t_point){1, 0, 0}) : 0;
	env->flags.move_u ? move(env, 1, (t_point){0, 1, 0}) : 0;
	env->flags.move_d ? move(env, -1, (t_point){0, 1, 0}) : 0;
	env->flags.rotate_l && ++e ? env->cam->rot.z -= 3 : 0;
	env->flags.rotate_r && ++e ? env->cam->rot.z += 3 : 0;
	e ? init_scene(env) : 0;
}
