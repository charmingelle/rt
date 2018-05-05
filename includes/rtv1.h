/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cozzmonavt <cozzmonavt@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/10 15:32:04 by cozzmonavt       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <stdio.h>
# include <time.h>
# include <errno.h>

# define CL_USE_DEPRECATED_OPENCL_1_2_APIS
# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else
#  include <CL/cl.h>
#  include <fcntl.h>
# endif

# include "sgl.h"
# include "structures.h"

# define PROGRAM_NAME "RTv1"

# define KERNEL_FOLDER "./resources/kernels/"

# define W_WIDTH 1440
# define W_HEIGHT 810

# define EXIT_KEY SDLK_ESCAPE

# define CAMERA_M(n)(n & 0b1)

typedef struct	s_cl_core
{
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	queue;
}				t_cl_core;

typedef struct	s_cl_kl
{
	cl_program			program;
	cl_kernel			kernel;
	cl_mem				mem;
}				t_cl_kl;

typedef struct	s_cam
{
	t_point		pos;
	t_rotate	rot;
	t_uint		rot_os;
	t_cl_kl		kl;
	t_viewport	*vwp;
}				t_cam;

typedef struct	s_scene
{
	cl_mem	objs;
	cl_mem	light;
	t_uint	objs_c;
	t_uint	light_c;
	t_obj	*objs_h;
	t_light	*light_h;
}				t_scene;

typedef struct	s_env
{
	t_cl_core		cl;
	t_sgl_window	*win;
	t_cam			*cam;
	t_scene			scene;
	t_ulong			flags;
}				t_env;

/*
**				Draw.c
**				↓↓↓↓↓↓
*/

void			render_scene(t_env *env);

/*
**				Events.c
**				↓↓↓↓↓↓↓↓
*/

int				poll_events(t_env *env);

/*
**				Scene.c
**				↓↓↓↓↓↓↓
*/

void			read_scene(t_env *env, int ac, char **av);

/*
**				Fps.c
**				↓↓↓↓↓
*/

void			display_fps(SDL_Renderer *rend);

/*
**				Utils.c
**				↓↓↓↓↓↓↓
*/

void			display_usage(t_uchar help);

void			camera_rotate(t_env *env);

void			resize_viewport(t_viewport *vwp, int width, int height);

void			init_env(t_env *env);

/*
**				OpenCl.c
**				↓↓↓↓↓↓↓↓
*/

void			cl_reinit_mem(t_cl_core *cl, cl_mem *mem,
								size_t size, void *ptr);

void			cl_init(t_cl_core *cl, cl_device_type dev_type);

void			cl_parse_kernel(t_cl_core *cl, t_cl_kl *kl,
						const char *kernel_name, const char *func_name);

/*
**				Cleaner.c
**				↓↓↓↓↓↓↓↓↓
*/

int				count_splited(char **arr);

void			free_splited(char **arr);

/*
**				Sys_handler.c
**				↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_err_handler(const char *msg, const char *add,
								int err, t_uchar stop);

#endif
