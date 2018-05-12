/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.cl                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:32:50 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/23 20:34:48 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h.cl"

/*---------------------------------UTILITIES---------------------------------*/

inline static float rand(uint *seed0, uint *seed1)
{
	*seed0 = 36969 * ((*seed0) & 65535) + ((*seed0) >> 16);
	*seed1 = 18000 * ((*seed1) & 65535) + ((*seed1) >> 16);

	unsigned int ires = ((*seed0) << 16) + (*seed1);
	union {
		float f;
		unsigned int ui;
	} res;

	res.ui = (ires & 0x007FFFFF) | 0x40000000;
	return ((res.f - 2.0F) / 2.0F);
}

inline float3	reflect_ray(float3 R, float3 N)
{
	return (2.0F * N * dot(N, R) - R);
}

inline float	deg_to_rad(float deg)
{
	return (deg * M_PI / 180.0F);
}

float3			avg_color(float3 arr[], uint num)
{
	uint	it = 0;
	float3	result = 0.0F;

	while(it < num)
		result += arr[it++];
	return (result / num);
}

float3			canvas_to_viewport(float x, float y, t_viewport vwp)
{
	return ((float3){x * vwp.vw_width / vwp.wd_width, y * vwp.vw_height / vwp.wd_height, vwp.dist});
}

float3			rotate_point(float3 rot, float3 D)
{
	float3	sin_c;
	float3	cos_c;
	float3	RX;
	float3	RY;
	float3	RZ;

	sin_c.x = sin(rot.x * M_PI / 180.0F);
	cos_c.x = cos(rot.x * M_PI / 180.0F);
	sin_c.y = sin(rot.y * M_PI / 180.0F);
	cos_c.y = cos(rot.y * M_PI / 180.0F);
	sin_c.z = sin(rot.z * M_PI / 180.0F);
	cos_c.z = cos(rot.z * M_PI / 180.0F);

	RX.x = D.x;
	RX.y = D.y * cos_c.x + D.z * sin_c.x;
	RX.z = D.z * cos_c.x - D.y * sin_c.x;

	RY.x = RX.x * cos_c.y - RX.z * sin_c.y;
	RY.y = RX.y;
	RY.z = RX.z * cos_c.y + RX.x * sin_c.y;

	RZ.x = RY.x * cos_c.z + RY.y * sin_c.z;
	RZ.y = RY.y * cos_c.z - RY.x * sin_c.z;
	RZ.z = RY.z;
	return (RZ);
}

float3			calc_normal(float3 P, float3 D, t_obj obj)
{
	float3		N;
	float3		T;

	N = P - obj.pos;
	if (obj.type == PLANE)
		return (obj.dir / fast_length(obj.dir));
	else if (obj.type == CYLINDER || obj.type == CONE)
	{
		if (obj.type == CONE)
			T = (obj.dir - P) / fast_length(obj.dir - P);
		else if (obj.type == CYLINDER)
			T = (obj.dir - obj.pos) / fast_length(obj.dir - obj.pos);
		N -= T * dot(N, T);
		N /= fast_length(N);
	}
	else if (obj.type == SPHERE)
		N /= fast_length(N);
	return (N);
}

float			fix_limits(float3 O, float3 D, float3 Va, t_obj obj, float ints)
{
	float3	Q;

	Q = O + D * ints;
	if (dot(Va, Q - obj.pos) > 0 && dot(Va, Q - obj.dir) < 0)
		return (ints);
	return (INFINITY);
}

/*-------------------------------INTERSECTIONS-------------------------------*/

float2			intersect_ray_plane(float3 O, float3 D, t_obj obj)
{
	float2	T;
	float	k1;
	float	k2;

	obj.dir /= fast_length(obj.dir);
	k1 = dot(D, obj.dir);
	k2 = dot(O - obj.pos, obj.dir);
	if (k1 != 0.0F)
	{
		T.x = -k2 / k1;
		T.y = INFINITY;
		return (T);
	}
	return ((float2){INFINITY, INFINITY});
}


float2			intersect_ray_cylinder(float3 O, float3 D, t_obj obj)
{
	float	descr;
	float	k1;
	float	k2;
	float	k3;
	float2	T;
	float3	D_Va;
	float3	OC_Va;
	float3	Va = (obj.dir - obj.pos) / fast_length(obj.dir - obj.pos);

	D_Va = D - dot(D, Va) * Va;
	OC_Va = (O - obj.dir) - dot((O - obj.dir), Va) * Va;
	k1 = dot(D_Va, D_Va);
	k2 = 2.0F * dot(D_Va, OC_Va);
	k3 = dot(OC_Va, OC_Va) - obj.rad * obj.rad;

	descr = k2 * k2 - 4.0F * k1 * k3;
	if (descr < 0)
		return ((float2){INFINITY, INFINITY});
	T = (float2){
		(-k2 - sqrt(descr)) / (2.0F * k1),
		(-k2 + sqrt(descr)) / (2.0F * k1)};
	T.x = fix_limits(O, D, Va, obj, T.x);
	T.y = fix_limits(O, D, Va, obj, T.y);
	return (T);
}

float2			intersect_ray_cone(float3 O, float3 D, t_obj obj)
{
	float	descr;
	float	alpha = deg_to_rad(obj.rad);
	float	k1;
	float	k2;
	float	k3;
	float	dva;
	float	ocva;
	float2	t_alpha = {sin(alpha), cos(alpha)};
	float2	T;
	float3	D_Va;
	float3	OC_Va;
	float3	Va = (obj.dir - obj.pos) / fast_length(obj.dir - obj.pos);

	dva = dot(D, Va);
	ocva = dot((O - obj.dir), Va);
	D_Va = D - dva * Va;
	OC_Va = (O - obj.dir) - ocva * Va;

	k1 = t_alpha.y * t_alpha.y * dot(D_Va, D_Va) - t_alpha.x * t_alpha.x * dva * dva;
	k2 = 2.0F * t_alpha.y * t_alpha.y * dot(D_Va, OC_Va) - 2.0F * t_alpha.x * t_alpha.x * dva * ocva;
	k3 = t_alpha.y * t_alpha.y * dot(OC_Va, OC_Va) - t_alpha.x * t_alpha.x * ocva * ocva;

	descr = k2 * k2 - 4.0F * k1 * k3;
	if (descr < 0)
		return ((float2){INFINITY, INFINITY});
	T = (float2){
		(-k2 + sqrt(descr)) / (2.0F * k1),
		(-k2 - sqrt(descr)) / (2.0F * k1)};
	T.x = fix_limits(O, D, Va, obj, T.x);
	T.y = fix_limits(O, D, Va, obj, T.y);
	return (T);
}

float2			intersect_ray_sphere(float3 O, float3 D, t_obj obj)
{
	float	descr;
	float	k1;
	float	k2;
	float	k3;
	float3	OC;

	OC = O - obj.pos;

	k1 = dot(D, D);
	k2 = 2.0F * dot(OC, D);
	k3 = dot(OC, OC) - obj.rad * obj.rad;

	descr = k2 * k2 - 4.0F * k1 * k3;
	if (descr < 0.0F)
		return ((float2){INFINITY, INFINITY});
	return ((float2){
		(-k2 - sqrt(descr)) / (2.0F * k1),
		(-k2 + sqrt(descr)) / (2.0F * k1)});
}

float2			choose_intersection(float3 O, float3 D, t_obj obj, int type)
{
	if (type == SPHERE)
		return (intersect_ray_sphere(O, D, obj));
	else if (type == PLANE)
		return (intersect_ray_plane(O, D, obj));
	else if (type == CYLINDER)
		return (intersect_ray_cylinder(O, D, obj));
	else if (type == CONE)
		return (intersect_ray_cone(O, D, obj));
	else
		return ((float2){INFINITY, INFINITY});
}

t_obj_data		closest_intersection(float3 O, float3 D, __constant t_obj *objs)
{
	t_obj_data	obj_data;
	float2		T;
	int			it = -1;

	obj_data.closest_t = INFINITY;
	obj_data.obj.type = -1;
	while (objs[++it].type >= 0)
	{
		T = choose_intersection(O, D, objs[it], objs[it].type);
		if (T.x > 0.0F && T.x < obj_data.closest_t)
		{
			obj_data.closest_t = T.x;
			obj_data.obj = objs[it];
		}
		if (T.y > 0.0F && T.y < obj_data.closest_t)
		{
			obj_data.closest_t = T.y;
			obj_data.obj = objs[it];
		}
	}
	return (obj_data);
}

/*--------------------------------PATHTRACING--------------------------------*/

float3			trace_ray(float3 O, float3 D, __constant t_obj *objs, uint *seed0, uint *seed1)
{
	t_obj_data	obj_data;

	float3		P;
	float3		N;
	float3		U;

	float3		final_color = 0.0F;
	float3		mask = 1.0F;

	float		r1, r2, r2s;

	int	bounce = -1;
	while (++bounce < BOUNCE_DEPTH)
	{
		obj_data = closest_intersection(O, D, objs);
		if (obj_data.obj.type < 0)
			return (final_color + mask * 0.1F * (bounce > 0 ? 1.0F : 0.0F));
		P = O + obj_data.closest_t * D;
		N = calc_normal(P, D, obj_data.obj);
		N = dot(N, D) < 0.0F? N : N * (-1.0F);

		if (obj_data.obj.refl)
		{
			O = P + N * 0.01F;
			D = reflect_ray(-D, N);
			obj_data.obj.color *= obj_data.obj.refl;
		}
		else
		{
			O = P + N * 0.03F;

			r1 = 2.0F * M_PI * rand(seed0, seed1);
			r2 = rand(seed0, seed1);
			r2s = sqrt(r2);

			U = fast_normalize(cross(fabs(N.x) > 0.1f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f), N));
			D = fast_normalize(U * cos(r1) * r2s + cross(N, U) * sin(r1) * r2s + N * sqrt(1.0f - r2));
		}
		final_color += mask * obj_data.obj.emission;

		mask *= obj_data.obj.color;
		mask *= dot(D, N);
		mask *= 2.0F;
	}
	return (final_color);
}


__kernel void
render_scene(
	__global	float3		*colors,
				float3		O,
				float3		rot,
				t_viewport	vwp,
	__constant	t_obj		*objs,
				uint		random_seed,
				uint		total_samples)
{
	short int	screen_x = get_global_id(0);
	short int	screen_y = get_global_id(1);
	short int	x = screen_x - vwp.wd_width / 2;
	short int	y = vwp.wd_height / 2 - screen_y;
	uint		seed0 = random_seed * screen_x;
	uint		seed1 = random_seed * screen_y;
	int			itx = -1;
	int			ity = -1;
	float3		color[SMOOTH_LEVEL * SMOOTH_LEVEL] = {0};
	float3		D;

	while (++itx < SMOOTH_LEVEL)
	{
		ity = -1;
		while (++ity < SMOOTH_LEVEL)
		{
			D = rotate_point(rot, canvas_to_viewport(x + (itx + 0.5) / SMOOTH_LEVEL, y + (ity + 0.5) / SMOOTH_LEVEL, vwp));
			color[ity * SMOOTH_LEVEL + itx] = trace_ray(O, D, objs, &seed0, &seed1);
		}
	}

	colors[screen_y * vwp.wd_width + screen_x] *= total_samples - 1;
	colors[screen_y * vwp.wd_width + screen_x] += avg_color(color, SMOOTH_LEVEL * SMOOTH_LEVEL);
	colors[screen_y * vwp.wd_width + screen_x] /= total_samples;
}
