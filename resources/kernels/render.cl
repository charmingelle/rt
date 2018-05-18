/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.cl                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:32:50 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/18 19:21:37 by grevenko         ###   ########.fr       */
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
	if (obj.type == PLANE || obj.type == DISC)
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
	else if (obj.type == CUBE)
	{
		float3 N_[6];
		float NP[6];
		//front side
		float3	P1 = {obj.pos.x, obj.pos.y, obj.pos.z};
		float3	P2 = {obj.pos.x, obj.pos.y + obj.rad, obj.pos.z};
		float3	P3 = {obj.pos.x + obj.rad, obj.pos.y + obj.rad, obj.pos.z};
		N_[0] = cross(P2 - P1, P3 - P1);
		NP[0] = dot(N_[0], P - P1);
		//back side
		N_[1] = cross(P3 - P1, P2 - P1);
		P1 = (float3){obj.dir.x, obj.dir.y, obj.dir.z};
		NP[1] = dot(N_[1], P - P1);
		//left side
		P1 = (float3){obj.dir.x, obj.dir.y, obj.dir.z};
		P2 = (float3){obj.dir.x, obj.dir.y + obj.rad, obj.dir.z};
		P3 = (float3){obj.pos.x, obj.pos.y, obj.pos.z};
		N_[2] = cross(P2 - P1, P3 - P1);
		NP[2] = dot(N_[2], P - P1);
		//right side
		N_[3] = cross(P3 - P1, P2 - P1);
		P1 = (float3){obj.dir.x + obj.rad, obj.dir.y, obj.dir.z};
		NP[3] = dot(N_[3], P - P1);
		//down side
		P1 = (float3){obj.dir.x, obj.dir.y, obj.dir.z};
		P2 = (float3){obj.dir.x + obj.rad, obj.dir.y, obj.dir.z};
		P3 = (float3){obj.pos.x, obj.pos.y, obj.pos.z};
		N_[4] = cross(P2 - P1, P3 - P1);
		NP[4] = dot(N_[4], P - P1);
		//up side
		N_[5] = cross(P3 - P1, P2 - P1);
		P1 = (float3){obj.dir.x, obj.dir.y + obj.rad, obj.dir.z};
		NP[5] = dot(N_[5], P - P1);

		int i  = -1;
		while (++i < 6)
			if (NP[i] > -EPSILON && NP[i] <= EPSILON)
				break;
		N = fast_normalize(N_[i]);
		return (N);
	}
	else if (obj.type == PARABOLID)
	{
		float3 coeff = {3.0F, 1.5F, 5.0F};
		N.x = 2.0F * N.x / coeff.x;
		N.y = -2.0f;
		N.z = 2.0F * N.z / coeff.z;
		N = fast_normalize(N);
		return (N);
	}
	else if (obj.type == HYPERBOLOID)
	{
		float	k;
		float3	A;

		k = dot(fast_normalize(obj.dir), N);
		A = fast_normalize(obj.dir) * k;
		A = N - A;
		k = dot(A, A);
		k = 1 / sqrt(k) * sqrt(obj.rad2);
		N = P - A - (obj.pos - A) * k / (sqrt(obj.rad2) + k);
		return (fast_normalize(N));
	}
	return (N);
}

float			fix_limits(float3 O, float3 D, float3 Va, t_obj obj, float ints)
{
	float3	Q;

	Q = O + D * ints;
	if (Q.y < obj.dir.y && obj.type == PARABOLID)
		return (ints);
	else if (Q.y > -(obj.dir.y - obj.pos.y * 2.0F) && Q.y < obj.dir.y && obj.type == HYPERBOLOID)
		return (ints);
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

float2			get_quadratic_solution(float a, float b, float c)
{
	float	discrim;
	float	t1;
	float	t2;

	discrim = b * b - 4 * a * c;
	if (discrim < 0)
		return ((float2){INFINITY, INFINITY});
	t1 = (-b + sqrt(discrim)) / (2 * a);
	t2 = (-b - sqrt(discrim)) / (2 * a);
	return ((float2){t1, t2});
}


float			get_canonic_cubic_solution(float p, float q)
{
	float	t;
	float	qu;
	float	alpha;
	float	beta;
	float	im;

	qu = (p / 3.0F) * (p / 3.0F) * (p / 3.0F) + (q / 2.0F) * (q / 2.0F);
	alpha = pow((-q / 2.0F) + pow(qu, 1.0F / 2.0F), 1.0F / 3.0F);
	beta = pow((-q / 2.0F) + pow(qu, 1.0F / 2.0F), 1.0F / 3.0F);
	im = pow(3.0F, 1.0F / 2.0F) * ((alpha - beta) / 2.0F);
	t = alpha + beta;
	return (t);
}

float			get_cubic_solution(float a, float b, float c, float d)
{
	float	q;
	float	p;

	q = (2.0F * b * b * b - 9.0F * a * b * c + 27.0F * a * a * d) / (27.0F * a * a * a);
	p = (3.0F * a * c - b * b) / (3.0F * a * a);
	return (get_canonic_cubic_solution(p, q));
}

float4			get_quartic_solution(float a, float b, float c, float d)
{
	float	y1;
	float	a_plus_minus;
	float	b_plus_minus;
	float	c_plus_minus;
	float2	res1;
	float2	res2;	
	float4	res;

	y1 = get_cubic_solution(a, b, c, d);
	a_plus_minus = ((a * a) / 4.0F) - b + y1;
	b_plus_minus = (a / 2.0F) * y1 - c;
	c_plus_minus = ((y1 * y1) / 4.0F) - d;
	res1 = get_quadratic_solution(1.0F + a_plus_minus, (a / 2.0F) + b_plus_minus, (y1 / 2.0F) + c_plus_minus);
	res2 = get_quadratic_solution(1.0F - a_plus_minus, (a / 2.0F) - b_plus_minus, (y1 / 2.0F) - c_plus_minus);
	res[0] = res1[0];
	res[1] = res1[1];
	res[2] = res2[0];
	res[3] = res2[1];
	return (res);
}

float2			intersect_ray_torus(float3 O, float3 D, t_obj obj)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	e;
	float	g;
	float	h;
	float	i;
	float	j;
	float	k;
	float	l;
	float4	res;
	
	g = 4.0F * obj.rad * obj.rad * (O.x * O.x + O.y * O.y);
	h = 8.0F * obj.rad * obj.rad * (D.x * O.x + D.y * O.y);
	i = 4.0F * obj.rad * obj.rad * (D.x * D.x + D.y * D.y);
	j = O.x * O.x + O.y * O.y + O.z * O.z;
	k = 2.0F * (D.x * O.x + D.y * O.y + D.z * O.z);
	l = D.x * D.x + D.y * D.y + D.z * D.z + obj.rad * obj.rad - obj.rad2 * obj.rad2;

	a = j * j;
	b = (2.0F * j * k) / a;
	c = (2.0F * j * l + k * k - g) / a;
	d = (2.0F * k * l - h) / a;
	e = (l * l - i) / a;

	res = get_quartic_solution(b, c, d, e);
	return (float2){min(res[0], res[1]), min(res[2], res[3])};
}

// ---

// float	solve_quartic(float a, float b, float c, float l, float e)
// {
// 	float	j, k, p, q, z;
// 	int		u, v, g;
// 	float	*r;
// 	float	x;
// 	int		i;

// 	r = (float *)malloc(sizeof(float) * 8);

// 	b /= a;
// 	c /= a;
// 	l /= a;
// 	e /= a;
// 	j = 3 * b;
// 	k = 2 * c;
// 	p = (12 * k - j * j) / 48;
// 	q = (2 * j * j * j - 36 * j * k + 432 * l) / 1728;
// 	z = q * q / 4 + p * p * p /27;

// 	u = 0;
// 	v = 0;
// 	for(g = 1; g < 4; g++)
// 	{
// 		r[g] = z > 0 | p == 0 ? cbrt(-q / 2 + sqrt(z)) + cbrt(-q / 2 - sqrt(z)) - j / 12 : sqrt(-p / .75) * cos(acos(-q / sqrt(-p * p * p * 4 / 27)) / 3 - g * acos(-.5)) -j / 12;
// 		r[g + 4] = (r[g] + b) * pow(r[g], 3) + c * r[g] * r[g] + l * r[g] + e;
// 		if (r[g + 4] > r[g + 3] | g == 1)
// 			u = g;
// 		if (r[g + 4] < r[g + 3] | g == 1)
// 			v = g;
// 	}
// 	if(r[v + 4] > 0)
// 		x = 0;
// 	if(r[v + 4] == 0)
// 		x = r[v];
// 	if(r[v + 4] < 0)
// 	{
// 		i = -1;
// 		x = 2 * r[v] - r[u] + (r[v] - r[u] == 0);
// 		while (++i < 99)
// 			x = x - ((x + b) * x * x * x + c * x * x + l * x + e) / (4 * x * x * x + j * x * x + k * x + l);
// 	}
// 	return (x);
// }

// float2			intersect_ray_torus(float3 O, float3 D, t_obj obj)
// {
// 	float	a;
// 	float	b;
// 	float	c;
// 	float	d;
// 	float	e;
// 	float	result;
// 	float	g;
// 	float	h;
// 	float	i;
// 	float	j;
// 	float	k;
// 	float	l;
// 	float4	x;
// 	float4	u;
// 	int		i;
// 	float	ret;
	
// 	g = 4 * obj.rad * obj.rad * (O.x * O.x + O.y * O.y);
// 	h = 8 * obj.rad * obj.rad * (D.x * O.x + D.y * O.y);
// 	i = 4 * obj.rad * obj.rad * (D.x * D.x + D.y * D.y);
// 	j = O.x * O.x + O.y * O.y + O.z * O.z;
// 	k = 2 * (D.x * O.x + D.y * O.y + D.z * O.z);
// 	l = D.x * D.x + D.y * D.y + D.z * D.z + obj.rad * obj.rad - obj.rad2 * obj.rad2;

// 	a = j * j;
// 	b = (2 * j * k) / a;
// 	c = (2 * j * l + k * k - g) / a;
// 	d = ((2 * k * l) − h) / a;
// 	e = ((l * l) − i) / a;

// 	u[0] = b;
// 	u[1] = c;
// 	u[2] = d;
// 	u[3] = e;
// 	count_roots = fourth_degree_equation(&x, u);
// 	i = -1;
// 	ret = INFINITY;
// 	while (count_roots > ++i)
// 		if (x[i] < ret && x[i] > EPSILON)
// 			ret = x[i];
// 	return (ret + t);
// 	result = solve_quartic(a, b, c, d, e);
// 	return (float2){result, result};
// }

// ---

// ---
// ---

// static void sort(float3 *ua, float2 *l)
// {
// 	if (fabs((*ua)[0]) > fabs((*ua)[1]) && fabs((*ua)[0]) > fabs((*ua)[2]))
// 	{
// 		(*l)[0] = (*ua)[0];
// 		(*l)[1] = fabs((*ua)[1]) > fabs((*ua)[2]) ? (*ua)[1] : (*ua)[2];
// 	}
// 	else if (fabs((*ua)[1]) > fabs((*ua)[0]) && fabs((*ua)[1]) > fabs((*ua)[2]))
// 	{
// 		(*l)[0] = (*ua)[1];
// 		(*l)[1] = fabs((*ua)[0]) > fabs((*ua)[2]) ? (*ua)[0] : (*ua)[2];
// 	}
// 	else
// 	{
// 		(*l)[0] = (*ua)[2];
// 		(*l)[1] = fabs((*ua)[0]) > fabs((*ua)[1]) ? (*ua)[0] : (*ua)[1];
// 	}
// }

// static void negative_discr_solution(t_equation *e)
// {
// 	float 	n;
// 	float 	bq3;
// 	float 	beta;
// 	float 	a3;
// 	float3 	ua;

// 	n = sqrt(e->b);
// 	bq3 = n * n * n;
// 	beta = (e->br / bq3 < 1.0f) ? acos(e->br / bq3) : 0.0f;
// 	a3 = -2.0f * n;
// 	ua[0] = a3 * cos(beta / 3.0f) - e->c / 3.0f;
// 	ua[1] = a3 * cos((beta + 2.0f * M_PI) / 3.0f) - e->c / 3.0f;
// 	ua[2] = a3 * cos((beta - 2.0f * M_PI) / 3.0f) - e->c / 3.0f;
// 	e->flag = false;
// 	sort(&ua, &(e->l));
// 	if (e->l[0] >= 0.0f)
// 	{
// 		e->real1 = sqrt(e->l[0]);
// 		e->im1 = 0.0f;
// 	}
// 	else
// 	{
// 		e->im1 = sqrt(-e->l[0]);
// 		e->real1 = 0.0f;
// 	}
// 	if (e->l[1] >= 0.0f)
// 	{
// 		e->im2 = 0.0f;
// 		e->real2 = sqrt(e->l[1]);
// 	}
// 	else
// 	{
// 		e->real2 = 0.0f;
// 		e->im2 = sqrt(-e->l[1]);
// 	}
// }

// static void positive_discr_solution(t_equation *e)
// {
// 	float 	n;
// 	float 	a3;
// 	float3 	ua;
// 	float 	n2;
// 	float 	u2;

// 	n = (e->br < 0.0f) ? -1.0f : 1.0f;
// 	a3 = -n * cbrt(fabs(e->br) + sqrt(e->discr));
// 	ua[0] = a3 + e->b / a3 - e->c / 3.0f;
// 	ua[1] = -0.5f * ((a3 * a3 + e->b) / a3) - e->c / 3.0f;
// 	ua[2] = -(sqrt(3.0f) / 2.0f) * fabs(a3 - (e->b / a3));
// 	e->flag = true;
// 	n2 = sqrt(sqrt(ua[1] * ua[1] + ua[2] * ua[2]));
// 	u2 = atan2(ua[2], ua[1]);
// 	e->real1 = n2 * cos(u2 * 0.5f);
// 	e->im1 = n2 * sin(u2 * 0.5f);
// 	e->real2 = e->real1;
// 	e->im2 = -e->im1;
// }

// static int	fourth_degree_equation(float4 *t, float4 a)
// {
// 	float	res;
// 	float	im_re1;
// 	float	im_re2;
// 	float	komp;
// 	t_equation 	e;

// 	e.aa = a[0] * a[0];
// 	e.pp = a[1] - 0.375f * e.aa;
// 	e.rr = a[3] - 0.25f * (a[0] * a[2] - 0.25f * e.aa * (a[1] - 0.1875f * e.aa));
// 	e.q2 = a[2] - 0.5f * a[0] * (a[1] - 0.25f * e.aa);
// 	e.c = 0.5f * e.pp;
// 	e.aa = 0.25f * (0.25f * e.pp * e.pp - e.rr);
// 	e.b = e.c * e.c / 9.0f - e.aa / 3.0f;
// 	e.br = e.c * e.c * e.c / 27.0f - e.c * e.aa / 6.0f - (0.125f * e.q2 * 0.125f * e.q2) / 2.0f;
// 	e.discr = ((e.br * e.br) - (e.b * e.b * e.b));
// 	if (e.discr < 0.0f)
// 		negative_discr_solution(&e);
// 	else
// 		positive_discr_solution(&e);
// 	im_re1 = e.im1 * e.im1 + e.real1 * e.real1;
// 	im_re2 = e.im2 * e.im2 + e.real2 * e.real2;
// 	komp = e.im1 * e.im2 - e.real1 * e.real2;
// 	res = e.q2 * 0.125f * komp / im_re1 / im_re2;
// 	(*t)[0] = e.real1 + e.real2 + res - a[0] * 0.25f;
// 	(*t)[1] = -e.real1 - e.real2 + res - a[0] * 0.25f;
// 	(*t)[2] = -e.real1 + e.real2 - res - a[0] * 0.25f;
// 	(*t)[3] = e.real1 - e.real2 - res - a[0] * 0.25f;
// 	if (!e.flag && e.l[0] >= 0.0f && e.l[1] >= 0.0f)
// 		return (4);
// 	else if (!e.flag)
// 		return (0);
// 	else
// 		return (2);
// }

// float2			intersect_ray_torus(float3 O, float3 D, t_obj obj)
// {
// 	int		count_roots;
// 	int		i;
// 	float4	u;
// 	float4	x;
// 	float	ret;
// 	float3 	OC;
// 	float4	qq;
// 	float4 	dots;

// 	OC = obj.pos - O;
// 	float b = dot(OC, D);
// 	float dis = b * b - dot(OC, OC) + (obj.rad + EPSILON) * M_PI;
// 	if (dis < 0)
// 		return INFINITY;
// 	dis = sqrt(dis);
// 	float t = b - dis;
// 	if (t < EPSILON)
// 	{
// 		t = b + dis;
// 		if (t < EPSILON)
// 			return (INFINITY);
// 	}
// 	O = O + D * (t - 2.f * EPSILON);
// 	OC = O - obj.pos;
// 	dots[0] = dot(OC, fast_normalize(obj.dir));
// 	dots[1] = dot(D, fast_normalize(obj.dir));
// 	dots[2] = dot(OC, OC);
// 	dots[3] = dot(OC, D);
// 	qq[0] = 1.0f - dots[1] * dots[1];
// 	qq[1] = 2.0f * (dots[3] - dots[0] * dots[1]);
// 	qq[2] = dots[2] - dots[0] * dots[0];
// 	qq[3] = dots[2] + obj.rad - obj.rad2;
// 	u[0] = 4.0f * dots[3];
// 	u[1] = 2.0f * qq[3] + u[0] * u[0] * 0.25f - 4.0f * obj.rad * qq[0];
// 	u[2] = u[0] * qq[3] - 4.0f * obj.rad * qq[1];
// 	u[3] = qq[3] * qq[3] - 4.0f * obj.rad * qq[2];
// 	count_roots = fourth_degree_equation(&x, u);
// 	i = -1;
// 	ret = INFINITY;
// 	while (count_roots > ++i)
// 		if (x[i] < ret && x[i] > EPSILON)
// 			ret = x[i];
// 	return ((float2){ret + t, ret + t});
// }

// ---
// ---

float2			intersect_ray_ellipsoid(float3 O, float3 D, t_obj obj)
{
	float	descr;
	float	k1;
	float	k2;
	float	k3;
	float3	OC;

	OC = O - obj.pos;
	float3 coeff = {3.0F, 1.5F, 5.0F};
	k1 = (D.x * D.x / coeff.x) + (D.y * D.y / coeff.y) + (D.z * D.z / coeff.z);
	k2 = (2.0F * OC.x * D.x / coeff.x) + (2.0F * OC.y * D.y / coeff.y) + (2.0F * OC.z * D.z / coeff.z);
	k3 =  (OC.x * OC.x / coeff.x) + (OC.y * OC.y / coeff.y) + (OC.z * OC.z / coeff.z) - obj.rad * obj.rad;

	descr = k2 * k2 - 4.0F * k1 * k3;
	if (descr < 0)
		return ((float2){INFINITY, INFINITY});
	return ((float2){
		(-k2 + sqrt(descr)) / (2.0F * k1),
		(-k2 - sqrt(descr)) / (2.0F * k1)});
}

float2			intersect_ray_parabolid(float3 O, float3 D, t_obj obj)
{
	float	descr;
	float	k1;
	float	k2;
	float	k3;
	float2	T;
	float3	OC;
	float3	Va = (obj.dir - obj.pos) / fast_length(obj.dir - obj.pos);

	OC = O - obj.pos;
	float3 coeff = {3.0F, 1.5F, 5.0F};
	k1 = (D.x * D.x / coeff.x) + (D.z * D.z / coeff.z);
	k2 = (2.0F * OC.x * D.x / coeff.x) + (2.0F * OC.z * D.z / coeff.z) - D.y;
	k3 =  (OC.x * OC.x / coeff.x) + (OC.z * OC.z / coeff.z) - OC.y * 2.0F;

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

float2			intersect_ray_hyperboloid(float3 O, float3 D, t_obj obj)
{
	float	descr;
	float	k1;
	float	k2;
	float	k3;
	float2	T;
	float3	OC;
	float3	Va = (obj.dir - obj.pos) / fast_length(obj.dir - obj.pos);

	OC = O - obj.dir;
	float3 coeff = {3.0F, 1.5F, 5.0F};
	k1 = (D.x * D.x / coeff.x) - (D.y * D.y / coeff.y) + (D.z * D.z / coeff.z);
	k2 = (2.0F * OC.x * D.x / coeff.x) - (2.0F * OC.y * D.y / coeff.y) + (2.0F * OC.z * D.z / coeff.z);
	k3 =  (OC.x * OC.x / coeff.x) - (OC.y * OC.y / coeff.y) + (OC.z * OC.z / coeff.z) - obj.rad;

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

float2			intersect_ray_disc(float3 O, float3 D, t_obj obj)
{
	float2	T = intersect_ray_plane(O, D, obj);

	if (T.x != INFINITY)
	{
		float3	P = O + D * T.x;
		float3	PC = P - obj.dir;
		float	k = dot(PC, PC);
        if (k <= obj.rad * obj.rad)
			return ((float2){T.x, INFINITY});
	}
	return ((float2){INFINITY, INFINITY});
}

inline static float	intersect_ray_rectangle(float3 P1, float3 P2, float3 P3, float3 P4, float3 O, float3 D)
{
	float3	Q = cross(P2 - P1, P4 - P1);
	float	F = -P1.x * (P2.y * P3.z - P3.y * P2.z) - P2.x * (P3.y * P1.z - P1.y * P3.z) - P3.x * (P1.y * P2.z - P2.y * P1.z);

	float	T = -(Q.x * O.x + Q.y * O.y + Q.z * O.z + F) / (Q.x * D.x + Q.y * D.y + Q.z * D.z);
	if (T < -2.0F)
		return (INFINITY);
	float3	P = O + D * T;

	float3	V1 = (P2 - P1) / fast_length(P2 - P1);
	float3	V2 = (P4 - P3) / fast_length(P4 - P3);
	float3	V3 = (P - P1) / fast_length(P - P1);
	float3	V4 = (P - P3) / fast_length(P - P3);

	float3	V5 = (P1 - P4) / fast_length(P1 - P4);
	float3	V6 = (P3 - P2) / fast_length(P3 - P2);
	float3	V7 = (P - P4) / fast_length(P - P4);
	float3	V8 = (P - P2) / fast_length(P - P2);

    if (dot(V1, V3) >= 0 && dot(V2, V4) >= 0 && dot(V5, V7) >= 0 && dot(V6, V8) >= 0)
		return (T);
	return (INFINITY);
}

float2			intersect_ray_cube(float3 O, float3 D, t_obj obj)
{
	//front
	float3	P1 = {obj.pos.x, obj.pos.y, obj.pos.z};
	float3	P2 = {obj.pos.x, obj.pos.y + obj.rad, obj.pos.z};
	float3	P3 = {obj.pos.x + obj.rad, obj.pos.y + obj.rad, obj.pos.z};
	float3	P4 = {obj.pos.x + obj.rad, obj.pos.y, obj.pos.z};
	float T = intersect_ray_rectangle(P1, P2, P3, P4, O, D);
	//back
	P1 = (float3){obj.dir.x, obj.dir.y, obj.dir.z};
	P2 = (float3){obj.dir.x, obj.dir.y + obj.rad, obj.dir.z};
	P3 = (float3){obj.dir.x + obj.rad, obj.dir.y + obj.rad, obj.dir.z};
	P4 = (float3){obj.dir.x + obj.rad, obj.dir.y, obj.dir.z};
	float T1 = intersect_ray_rectangle(P1, P2, P3, P4, O, D);
	T1 < T ? T = T1 : 0;
	// //left
	P1 = (float3){obj.dir.x, obj.dir.y, obj.dir.z};
	P2 = (float3){obj.dir.x, obj.dir.y + obj.rad, obj.dir.z};
	P3 = (float3){obj.pos.x, obj.pos.y + obj.rad, obj.pos.z};
	P4 = (float3){obj.pos.x, obj.pos.y, obj.pos.z};
	float T2 = intersect_ray_rectangle(P1, P2, P3, P4, O, D);
	// //right
	P1 = (float3){obj.dir.x + obj.rad, obj.dir.y, obj.dir.z};
	P2 = (float3){obj.dir.x + obj.rad, obj.dir.y + obj.rad, obj.dir.z};
	P3 = (float3){obj.pos.x + obj.rad, obj.pos.y + obj.rad, obj.pos.z};
	P4 = (float3){obj.pos.x + obj.rad, obj.pos.y, obj.pos.z};
	float T3 = intersect_ray_rectangle(P1, P2, P3, P4, O, D);
	T3 < T2 ? T2 = T3 : 0;
	// //down
	P1 = (float3){obj.dir.x, obj.dir.y, obj.dir.z};
	P2 = (float3){obj.dir.x + obj.rad, obj.dir.y, obj.dir.z};
	P3 = (float3){obj.pos.x + obj.rad, obj.pos.y, obj.pos.z};
	P4 = (float3){obj.pos.x, obj.pos.y, obj.pos.z};
	float T4 = intersect_ray_rectangle(P1, P2, P3, P4, O, D);
	// //up
	P1 = (float3){obj.dir.x, obj.dir.y + obj.rad, obj.dir.z};
	P2 = (float3){obj.dir.x + obj.rad, obj.dir.y + obj.rad, obj.dir.z};
	P3 = (float3){obj.pos.x + obj.rad, obj.pos.y + obj.rad, obj.pos.z};
	P4 = (float3){obj.pos.x, obj.pos.y + obj.rad, obj.pos.z};
	float T5 = intersect_ray_rectangle(P1, P2, P3, P4, O, D);
	T5 < T4 ? T4 = T5 : 0;

	T2 < T ? T = T2 : 0;
	T4 < T ? T = T4 : 0;
	return ((float2)(T, INFINITY));
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
	else if (type == ELLIPSOID)
		return(intersect_ray_ellipsoid(O, D, obj));
	else if (type == PARABOLID)
		return(intersect_ray_parabolid(O, D, obj));
	else if (type == HYPERBOLOID)
		return(intersect_ray_hyperboloid(O, D, obj));
	else if (type == DISC)
		return(intersect_ray_disc(O, D, obj));
	else if (type == CUBE)
		return(intersect_ray_cube(O, D, obj));
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

		if (obj_data.obj.material == REFLECT)
		{
			O = P + N * 0.01F;
			D = reflect_ray(-D, N);
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
