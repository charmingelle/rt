/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:32:50 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 12:09:43 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		process_colors(t_env *env)
{
	int		it;
	uint	*pixels;

	pixels = (uint *)env->win->surf->pixels;
	it = -1;
	while (++it < env->win->w * env->win->h)
		pixels[it] = TOHEX(TORGB(env->scene.colors_h[it].x),
							TORGB(env->scene.colors_h[it].y),
							TORGB(env->scene.colors_h[it].z));
}

void		init_scene(t_env *env)
{
	cl_int		err;
	cl_kernel	kl;

	env->scene.sampls = 0;
	env->scene.timestamp = SDL_GetTicks();
	err = 0;
	kl = env->cam->kl.kernel;
	env->scene.colors_h ? free(env->scene.colors_h) : 0;
	env->scene.colors_h = malloc(sizeof(cl_float3) * env->win->w * env->win->h);
	cl_reinit_mem(&env->cl, &env->cam->kl.mem,
					sizeof(cl_float3) * env->win->w * env->win->h, 0);
	clEnqueueFillBuffer(env->cl.queue, env->cam->kl.mem, &err, sizeof(cl_int),
		0, sizeof(cl_float3) * env->win->w * env->win->h, 0, 0, 0);
	err |= clSetKernelArg(kl, 0, sizeof(cl_mem), &env->cam->kl.mem);
	err |= clSetKernelArg(kl, 1, sizeof(cl_float3), &env->cam->pos);
	err |= clSetKernelArg(kl, 2, sizeof(cl_float3), &env->cam->rot);
	err |= clSetKernelArg(kl, 3, sizeof(t_viewport), env->cam->vwp);
	cl_reinit_mem(&env->cl, &env->scene.objs,
		sizeof(t_obj) * (env->scene.objs_c + 1), env->scene.objs_h);
	err |= clSetKernelArg(kl, 4, sizeof(cl_mem), &env->scene.objs);
	err ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
}

void		render_scene(t_env *env)
{
	cl_kernel	kl;
	uint		random_seed;
	cl_int		err;
	int			it;

	kl = env->cam->kl.kernel;
	env->scene.sampls++;
	err = 0;
	it = -1;
	random_seed = rand();
	err |= clSetKernelArg(kl, 5, sizeof(uint), &random_seed);
	err |= clSetKernelArg(kl, 6, sizeof(uint), &env->scene.sampls);
	err |= clEnqueueNDRangeKernel(env->cl.queue, kl, 2, NULL,
		(size_t[3]){env->win->w, env->win->h, 0}, NULL, 0, NULL, NULL);
	err |= clEnqueueReadBuffer(env->cl.queue, env->cam->kl.mem, CL_TRUE, 0,
							sizeof(cl_float3) * env->win->w * env->win->h,
							env->scene.colors_h, 0, 0, 0);
	err ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	process_colors(env);
}
