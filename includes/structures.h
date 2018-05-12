/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grevenko <grevenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 13:21:23 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 18:24:00 by grevenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef struct	s_light
{
	char		type;
	float		intens;
	cl_float3	pos;
}				t_light;

typedef struct	s_obj
{
	cl_float3	color;
	short int	type;
	cl_float3	pos;
	cl_float3	dir;
	cl_float3	emission;
	float		rad;
	int			spec;
	float		refl;
	float		transp;
	char		*text;
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
