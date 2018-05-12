/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 14:17:41 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <stdio.h>
# include <time.h>
# include <errno.h>
# include <fcntl.h>

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

# define RANGE(x, l1, l2)(x < l1 ? l1 : x > l2 ? l2 : x)
# define TORGB(coef)((uint)(RANGE(coef, 0.0F, 1.0F) * 255.0F + 0.5F))
# define TOHEX(r, g, b)((uint)r * 0x10000 + (uint)g * 0x100 + (uint)b)

# define INTERACTIVE_M(n)(n & 0b1)
# define MOVE_F(n)(n & 0b10 && INTERACTIVE_M(n))
# define MOVE_B(n)(n & 0b100 && INTERACTIVE_M(n))

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
	cl_float3	pos;
	cl_float3	rot;
	uint		rot_os;
	t_cl_kl		kl;
	t_viewport	*vwp;
}				t_cam;

typedef struct	s_scene
{
	cl_mem		objs;
	cl_mem		light;
	uint		objs_c;
	uint		light_c;
	t_obj		*objs_h;
	t_light		*light_h;
	cl_mem		colors;
	cl_float3	*colors_h;
	uint		sampls;
	uint		timestamp;
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

void			init_scene(t_env *env);

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

// void			read_scene(t_env *env, int ac, char **av);

/*
**				Stats.c
**				↓↓↓↓↓↓↓
*/

void			display_fps(SDL_Renderer *rend);

void			display_stats(t_env *env, SDL_Renderer *rend);

/*
**				Movement.c
**				↓↓↓↓↓↓↓↓↓↓
*/

void			move(t_env *env, char dir);

void			handle_movement(SDL_Scancode key, t_env *env, char state);

/*
**				Vec.c
**				↓↓↓↓↓
*/

cl_float3		vec_add(cl_float3 v1, cl_float3 v2);

cl_float3		vec_sub(cl_float3 v1, cl_float3 v2);

cl_float3		vec_mult_num(cl_float3 v1, float num);

/*
**				Utils.c
**				↓↓↓↓↓↓↓
*/

cl_float3		tofloat3(t_point point);

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

void			quit_msg(t_env *env);

int				count_splited(char **arr);

void			free_splited(char **arr);

/*
**				Sys_handler.c
**				↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_err_handler(const char *msg, const char *add,
								int err, t_uchar stop);

/*
**				Anna
*/

void			read_scene(char *name, t_env *env);
char			*get_object_or_array_string(char *string, int type, void *place_to_save);
char			*parse_text_as(char *string, int type, void *place_to_save);

#endif
