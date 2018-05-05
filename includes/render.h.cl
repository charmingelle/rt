/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h.cl                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 13:16:51 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/24 13:19:46 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# define SPHERE 0
# define CYLINDER 1
# define CONE 2
# define PLANE 3

# define SMOOTH_LEVEL 1
# define REFLECT_DEPTH 6

typedef unsigned char	t_uchar;
typedef unsigned int	t_uint;
typedef unsigned long	t_ulong;
typedef unsigned short	t_ushort;

typedef struct	s_point
{
	double	x;
	double	y;
	double	z;
}				t_point;

typedef struct	s_rotate
{
	double	rx;
	double	ry;
	double	rz;
}				t_rotate;

# include "structures.h"

typedef struct	s_obj_data
{
	t_obj	obj;
	float	closest_t;
}				t_obj_data;

/*---------------------------------UTILITIES----------------------------------*/

float3		reflect_ray(float3 R, float3 N);
float		deg_to_rad(float deg);
t_uint		avg_color(t_uint arr[], t_uint num);
float3		canvas_to_viewport(float x, float y, t_viewport vwp,
								t_uint w_width, t_uint w_height);
float3		rotate_point(float3 rot, float3 D);
float3		calc_normal(float3 P, float3 D, t_obj obj);
float3		sum_colors(float3 a, float3 b);
float		fix_limits(float3 O, float3 D, float3 Va, t_obj obj, float ints);

/*-------------------------------INTERSECTIONS--------------------------------*/

float2		intersect_ray_plane(float3 O, float3 D, t_obj obj);
float2		intersect_ray_cylinder(float3 O, float3 D, t_obj obj);
float2		intersect_ray_cone(float3 O, float3 D, t_obj obj);
float2		intersect_ray_sphere(float3 O, float3 D, t_obj obj);
float2		choose_intersection(float3 O, float3 D, t_obj obj, int type);
t_obj_data	closest_intersection(float3 O, float3 D, float min, float max,
								__constant t_obj *objs);

/*-----------------------------------LIGHT------------------------------------*/

float		compute_lighting(float3 P, float3 N, float3 O, float3 V, int s,
							__constant t_light *light, __constant t_obj *objs);

/*---------------------------------RAYTRACING---------------------------------*/

t_uint		trace_ray(float3 O, float3 D, float min, float max,
						__constant t_obj *objs, __constant t_light *light);

/*-----------------------------------KERNEL-----------------------------------*/

__kernel
void		render_scene(__global t_uint *pixels, t_point cam_pos, t_rotate cam_rot,
						t_uint w_width, t_uint w_height, t_viewport vwp,
						__constant t_obj *objs, __constant t_light *light);

#endif
