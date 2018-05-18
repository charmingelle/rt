/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/18 19:22:35 by grevenko         ###   ########.fr       */
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
# define IMAGES_FOLDER "./resources/images/"

# define W_WIDTH 1440
# define W_HEIGHT 810

# define EXIT_KEY SDLK_ESCAPE

# define RANGE(x, l1, l2)(x < l1 ? l1 : x > l2 ? l2 : x)
# define TORGB(coef)((uint)(RANGE(coef, 0.0F, 1.0F) * 255.0F + 0.5F))
# define TOHEX(r, g, b)((uint)r * 0x10000 + (uint)g * 0x100 + (uint)b)

typedef struct	s_flag
{
	uint	move_m : 1;
	uint	move_f : 1;
	uint	move_b : 1;
	uint	sidle_l : 1;
	uint	sidle_r : 1;
	uint	move_u : 1;
	uint	move_d : 1;
	uint	sprint : 1;
	uint	rotate_l : 1;
	uint	rotate_r : 1;
}				t_flag;

typedef struct	s_cl_core
{
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	queue;
}				t_cl_core;

typedef struct	s_cl_kl
{
	cl_program	program;
	cl_kernel	kernel;
	cl_mem		mem;
}				t_cl_kl;

typedef struct	s_cam
{
	cl_float3	pos;
	cl_float3	rot;
	t_cl_kl		kl;
	t_viewport	*vwp;
}				t_cam;

typedef struct	s_scene
{
	cl_mem		objs;
	uint		objs_c;
	t_obj		*objs_h;
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
	t_flag			flags;
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
**				Scene_1.c
**				↓↓↓↓↓↓↓↓↓
*/

# define ERR(s) ft_err_handler(s, 0, 0, 1)

# define FIND_FUNC(n, s, f, st, d){if(!ft_strcmp(n,s)){f(st, d);break;}}

# define SOB(s){**s!='{'?ERR("Open '{' is missing"):0;++*s;}
# define SCB(s){**s!='}'?ERR("Closing '}' is missing"):0;++*s;}
# define SOBK(s){**s!='['?ERR("Open '[' is missing"):0;++*s;}
# define SCBK(s){**s!=']'?ERR("Closing ']' is missing"):0;++*s;}
# define SC(s, c){c!=','&&c!='}'&&c!=']'?ERR("',' is missing"):0;c==','?++*s:0;}
# define GCI(s)({int i=-1;do{i++;}while((s)[i]&&(s)[i]!=':');i;})

int				get_primitive_end(char *string);

void			read_scene(char *name, int ac, t_env *env);

char			*get_object_or_array_string(char *string, int type,
											void *place_to_save);

char			*parse_text_as(char *string, int type, void *place_to_save);

/*
**				Scene_2.c
**				↓↓↓↓↓↓↓↓↓
*/

void			parse_float(char **string, float *pts);

void			parse_string(char **string, char **pts);

void			parse_figure_type(char **string, short int *pts);

void			parse_material(char **string, t_material *pts);

void			parse_cl_float3(char **string, cl_float3 *pts);

/*
**				Scene_3.c
**				↓↓↓↓↓↓↓↓↓
*/

void			parse_scene(char **string, t_env *env);

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

void			move(t_env *env, char state, t_point dir);

void			movement_keys(SDL_Scancode key, t_env *env, char state);

void			handle_movement(t_env *env);

/*
**				Vec.c
**				↓↓↓↓↓
*/

cl_float3		vec_add(cl_float3 v1, cl_float3 v2);

cl_float3		vec_sub(cl_float3 v1, cl_float3 v2);

cl_float3		vec_mult_num(cl_float3 v1, float num);

/*
**				Utils_1.c
**				↓↓↓↓↓↓↓↓↓
*/

cl_float3		tofloat3(t_point point);

t_point			topoint(cl_float3 point);

void			display_usage(t_uchar help);

void			resize_viewport(t_viewport *vwp, int width, int height);

void			init_env(t_env *env);

/*
**				Utils_2.c
**				↓↓↓↓↓↓↓↓↓
*/

void			screenshot(t_env *env, char *name);

int				count_splited(char **arr);

void			free_splited(char **arr);

void			quit_msg(t_env *env);

/*
**				Obj_info.c
**				↓↓↓↓↓↓↓↓↓↓
*/

void			draw_info(SDL_Rect *p, SDL_Renderer *rend);

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
**				Sys_handler.c
**				↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_err_handler(const char *msg, const char *add,
								int err, t_uchar stop);

/*
**				Anna: added to utils_2.c for torus to work
**				↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

// float			vlen(cl_float3 a);
// cl_float3		vmult(cl_float3 a, float num);
// cl_float3		normalize(cl_float3 v);

#endif
