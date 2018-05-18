/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 13:21:23 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/18 19:22:42 by grevenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef enum	e_material
{
	DIFFUSE = 0,
	EMISSIVE,
	REFLECT,
	TRANSPARENT
}				t_material;

typedef struct	s_obj
{
	cl_float3	color;
	short int	type;
	cl_float3	pos;
	cl_float3	dir;
	cl_float3	emission;
	float		rad;
	float		rad2;
	t_material	material;
}				t_obj;

typedef struct	s_viewport
{
	short int	wd_width;
	short int	wd_height;
	float		vw_width;
	float		vw_height;
	float		dist;
}				t_viewport;

#endif
