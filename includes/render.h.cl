/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h.cl                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 13:16:51 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/18 19:22:39 by grevenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# define SPHERE 0
# define CYLINDER 1
# define CONE 2
# define PLANE 3
# define ELLIPSOID 4
# define PARABOLID 5
# define HYPERBOLOID 6
# define DISC 7
# define CUBE 8
# define TORUS 9
# define MEBIUS 10

# define SMOOTH_LEVEL 1
# define BOUNCE_DEPTH 4

__constant float EPSILON = 0.00003f;

typedef unsigned char	t_uchar;
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

typedef float3	cl_float3;

# include "structures.h"

typedef struct	s_obj_data
{
	t_obj	obj;
	float	closest_t;
}				t_obj_data;

typedef	struct			s_equation
{
	bool				flag;
	float				aa;
	float				pp;
	float				rr;
	float				q2;
	float				c;
	float				b;
	float				br;
	float				discr;
	float				real1;
	float				real2;
	float				im1;
	float				im2;
	float2				l;
}						t_equation;

/*---------------------------------UTILITIES----------------------------------*/

float3		reflect_ray(float3 R, float3 N);
float		deg_to_rad(float deg);
float3		avg_color(float3 arr[], uint num);
float3		canvas_to_viewport(float x, float y, t_viewport vwp);
float3		rotate_point(float3 rot, float3 D);
float3		calc_normal(float3 P, float3 D, t_obj obj);
float		fix_limits(float3 O, float3 D, float3 Va, t_obj obj, float ints);

/*-------------------------------INTERSECTIONS--------------------------------*/


float2		intersect_ray_plane(float3 O, float3 D, t_obj obj);
float2		intersect_ray_cylinder(float3 O, float3 D, t_obj obj);
float2		intersect_ray_cone(float3 O, float3 D, t_obj obj);
float2		intersect_ray_sphere(float3 O, float3 D, t_obj obj);
float2		intersect_ray_ellipsoid(float3 O, float3 D, t_obj obj);
float2		intersect_ray_parabolid(float3 O, float3 D, t_obj obj);
float2		intersect_ray_hyperboloid(float3 O, float3 D, t_obj obj);
float2		intersect_ray_disc(float3 O, float3 D, t_obj obj);
float2		intersect_ray_cube(float3 O, float3 D, t_obj obj);
float2		choose_intersection(float3 O, float3 D, t_obj obj, int type);
t_obj_data	closest_intersection(float3 O, float3 D, __constant t_obj *objs);

<<<<<<< HEAD
float2		get_quadratic_solution(float a, float b, float c);
float		get_canonic_cubic_solution(float p, float q);
float		get_cubic_solution(float a, float b, float c, float d);
float4		get_quartic_solution(float a, float b, float c, float d);
float2		intersect_ray_torus(float3 O, float3 D, t_obj obj);

=======
>>>>>>> anna
/*--------------------------------PATHTRACING---------------------------------*/

float3		trace_ray(float3 O, float3 D, __constant t_obj *objs, uint *seed0, uint *seed1);

/*-----------------------------------KERNEL-----------------------------------*/

__kernel void
render_scene(
	__global	float3		*colors,
				float3		O,
				float3		rot,
				t_viewport	vwp,
	__constant	t_obj		*objs,
				uint		random_seed,
				uint		total_samples);

#endif
