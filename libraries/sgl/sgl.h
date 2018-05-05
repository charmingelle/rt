/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sgl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/05 17:31:29 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/13 16:19:34 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Simple Graphics Library
**	Copyright (C) 2018  Pavlo Gritsenko
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SGL_H
# define SGL_H

# include "libft.h"

# ifdef linux
#  include <SDL2/SDL.h>
#  include <SDL2/SDL_ttf.h>
#  include <SDL2/SDL_image.h>
#  include <SDL2/SDL_mixer.h>
# else
#  include "SDL.h"
#  include "SDL_ttf.h"
#  include "SDL_image.h"
#  include "SDL_mixer.h"
# endif

# define ICON_PATH "./resources/images/icon.png"
# define FONT_PATH "./resources/fonts/"

typedef struct	s_font
{
	char		*name;
	int			size;
	t_uint		color;
	TTF_Font	*ttf;
}				t_font;

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

typedef struct	s_window
{
	int				w;
	int				h;
	char			*title;
	t_uint			id;
	SDL_Window		*p;
	SDL_Renderer	*rend;
	SDL_Texture		*tex;
	SDL_Surface		*surf;
}				t_sgl_window;

void			sgl_init(void);

int				sgl_quit(void);

void			sgl_plot_rend(t_point p, t_uint color, SDL_Renderer *rend);

void			sgl_plot_surf(t_point p, t_uint color, SDL_Surface *surf);

t_uint			sgl_get_tex(SDL_Texture *tex, int x, int y);

t_uint			sgl_get_surf(SDL_Surface *surf, int x, int y);

void			sgl_draw_line(t_point a, t_point b, t_uint color,
								SDL_Renderer *rend);

void			sgl_draw_str(const char *string, t_font *font_data,
								t_point align, SDL_Renderer *rend);

t_sgl_window	*sgl_new_window(const char *title, int width,
								int height, t_uint wm);

void			sgl_win_resize(t_uint win_id, t_uint new_width,
								t_uint new_height);

t_sgl_window	*sgl_get_window(const char *title);

t_sgl_window	*sgl_get_window_by_id(t_uint id);

TTF_Font		*sgl_get_font(t_font *font_data);

void			sgl_render_surface(SDL_Renderer *renderer, SDL_Surface *surface,
									t_point align);

void			sgl_vsync(void);

t_point			sgl_atop(const char *src);

int				sgl_check_point(t_point src);

t_point			sgl_rotate_point(t_point p, t_rotate angle);

#endif
