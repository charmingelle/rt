/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:32:50 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/13 16:18:45 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		render_scene(t_env *env)
{
	cl_int	err;

	(err = clSetKernelArg(env->cam->kl.kernel, 0, sizeof(cl_mem),
		&env->cam->kl.mem)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 1, sizeof(t_point),
		&env->cam->pos)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 2, sizeof(t_rotate),
		&env->cam->rot)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 3, sizeof(t_uint),
		&env->win->w)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 4, sizeof(t_uint),
		&env->win->h)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 5, sizeof(t_viewport),
		env->cam->vwp)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 6, sizeof(cl_mem),
		&env->scene.objs)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 7, sizeof(cl_mem),
		&env->scene.light)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	err = clEnqueueNDRangeKernel(env->cl.queue, env->cam->kl.kernel, 2, NULL,
		(size_t[3]){env->win->w, env->win->h, 0}, NULL, 0, NULL, NULL);
	err ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	err = clEnqueueReadBuffer(env->cl.queue, env->cam->kl.mem, CL_TRUE, 0,
		env->win->surf->pitch * env->win->h, env->win->surf->pixels, 0, 0, 0);
}
