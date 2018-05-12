/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/12 11:52:31 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 14:10:26 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	move(t_env *env, char dir)
{
	const float	speed = 0.1F;
	t_point		tmp;
	cl_float3	d;

	tmp.x = env->cam->vwp->vw_width / env->cam->vwp->wd_width;
	tmp.y = env->cam->vwp->vw_height / env->cam->vwp->wd_height;
	tmp.z = env->cam->vwp->dist;
	d = tofloat3(sgl_rotate_point(tmp,
		(t_rotate){env->cam->rot.x, env->cam->rot.y, env->cam->rot.z}));
	env->cam->pos = dir > 0 ? vec_add(env->cam->pos, vec_mult_num(d, speed))
							: vec_sub(env->cam->pos, vec_mult_num(d, speed));
	init_scene(env);
}

void	handle_movement(SDL_Scancode key, t_env *env, char state)
{
	if (state)
	{
		key == SDL_SCANCODE_W ? env->flags |= 0b10 : 0;
		key == SDL_SCANCODE_S ? env->flags |= 0b100 : 0;
	}
	else
	{
		key == SDL_SCANCODE_W ? env->flags &= ~0b10 : 0;
		key == SDL_SCANCODE_S ? env->flags &= ~0b100 : 0;
	}
}
