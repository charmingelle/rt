/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:16:00 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/12 18:15:01 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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

inline void	camera_rotate(t_env *env)
{
	env->cam->rot.ry = (int)(env->cam->rot.ry + 1) % 360;
	env->cam->pos.x = env->cam->rot_os * sin(ft_degtorad(env->cam->rot.ry));
	env->cam->pos.z = -(env->cam->rot_os * cos(ft_degtorad(env->cam->rot.ry)));
}

void		resize_viewport(t_viewport *vwp, int width, int height)
{
	if (width > height)
	{
		vwp->h = 1.0;
		vwp->w = (double)width / height;
	}
	else
	{
		vwp->w = 1.0;
		vwp->h = (double)height / width;
	}
}

void		init_env(t_env *e)
{
	ft_bzero(e, sizeof(t_env));
	e->win = sgl_new_window(PROGRAM_NAME, W_WIDTH, W_HEIGHT,
								SDL_WINDOW_RESIZABLE);
	e->cam = ft_memalloc(sizeof(t_cam));
	e->cam->vwp = ft_memalloc(sizeof(t_viewport));
	resize_viewport(e->cam->vwp, e->win->w, e->win->h);
	e->cam->vwp->dist = 1;
	cl_init(&e->cl, CL_DEVICE_TYPE_GPU);
	cl_parse_kernel(&e->cl, &e->cam->kl,
		KERNEL_FOLDER"render.cl", "render_scene");
	cl_reinit_mem(&e->cl, &e->cam->kl.mem, e->win->surf->pitch * e->win->h, 0);
}
