/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:16:00 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 14:15:53 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3	tofloat3(t_point point)
{
	cl_float3	tmp;

	tmp.x = (float)point.x;
	tmp.y = (float)point.y;
	tmp.z = (float)point.z;
	return (tmp);
}

inline void	display_usage(t_uchar help)
{
	ft_printf("Usage: ./RTv1 [scene file]\n");
	!help ? exit(0) : 0;
	ft_printf("{cyan}Scene file format:{nc}\n"
"  [Objects on scene] [Light on scene]\n"
"  [{$camera_x;$camera_y;$camera_z}, {$camera_rx;$camera_ry;$camera_rz},"
											" $camera_rotation_offset]\n"
"  \n{cyan}Objects and Light:{nc}\n"
"  {red}For sphere:{nc} sphere[$color, {$x;$y;$z}, $radius,"
											" $specular, $reflect]\n"
"  {red}For cylinder:{nc} cylinder[$color, {$x1;$y1;$z1}, {$x2;$y2;$z2},"
											" $radius, $specular, $reflect]\n"
"  {red}For cone:{nc} cone[$color, {$x1;$y1;$z1}, {$x2;$y2;$z2},"
											" $radius, $specular, $reflect]\n"
"  {red}For plane:{nc} plane[$color, {$x;$y;$z}, {$Nx;$Ny;$Nz},"
											" $specular, $reflect]\n");
	exit(0);
}

void		resize_viewport(t_viewport *vwp, int width, int height)
{
	if (width > height)
	{
		vwp->vw_height = 1.0;
		vwp->vw_width = (double)width / height;
	}
	else
	{
		vwp->vw_width = 1.0;
		vwp->vw_height = (double)height / width;
	}
	vwp->wd_width = width;
	vwp->wd_height = height;
}

void		init_env(t_env *env)
{
	srand(time(NULL));
	ft_bzero(env, sizeof(t_env));
	env->win = sgl_new_window(PROGRAM_NAME, W_WIDTH, W_HEIGHT,
								SDL_WINDOW_RESIZABLE);
	env->cam = ft_memalloc(sizeof(t_cam));
	env->cam->vwp = ft_memalloc(sizeof(t_viewport));
	resize_viewport(env->cam->vwp, env->win->w, env->win->h);
	env->cam->vwp->dist = 1;
	cl_init(&env->cl, CL_DEVICE_TYPE_GPU);
	cl_parse_kernel(&env->cl, &env->cam->kl,
		KERNEL_FOLDER"render.cl", "render_scene");
	cl_reinit_mem(&env->cl, &env->cam->kl.mem,
		sizeof(cl_float3) * env->win->w * env->win->h, 0);
}
