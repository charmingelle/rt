/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 13:21:23 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/06 17:42:14 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef struct	s_light
{
	char	type;
	float	intens;
	t_point	pos;
}				t_light;

typedef struct	s_obj
{
	t_uint		color;
	short int	type;
	t_point		pos;
	t_point		dir;
	float		rad;
	int			spec;
	float		refl;
}				t_obj;

typedef struct	s_viewport
{
	float	w;
	float	h;
	float	dist;
}				t_viewport;

#endif
