/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/12 12:10:52 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 14:10:37 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3	vec_add(cl_float3 v1, cl_float3 v2)
{
	return ((cl_float3){.x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z});
}

cl_float3	vec_sub(cl_float3 v1, cl_float3 v2)
{
	return ((cl_float3){.x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z});
}

cl_float3	vec_mult_num(cl_float3 v1, float num)
{
	return ((cl_float3){.x = v1.x * num, .y = v1.y * num, .z = v1.z * num});
}
