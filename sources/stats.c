/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/10 20:05:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/12 11:29:38 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	display_fps(SDL_Renderer *rend)
{
	char		s[512];
	static uint	frame_rate;
	uint		t_c;
	static uint	t_p;
	static uint	c_b;

	t_c = time(NULL);
	if (t_c - t_p && (t_p = t_c))
		frame_rate = 1000.0 / (SDL_GetTicks() - c_b);
	c_b = SDL_GetTicks();
	sprintf(s, "FPS: %u", frame_rate);
	sgl_draw_str(s, &(t_font){"SourceSans", 24, 0xFFFFFF, 0},
							(t_point){10, 5, 0}, rend);
}

void	display_render_time(t_env *env, SDL_Renderer *rend)
{
	char	s[512];
	float	sec;
	uint	min;
	uint	hours;
	uint	d;

	sec = (SDL_GetTicks() - env->scene.timestamp) / 1000.0F;
	min = (uint)sec / 60;
	hours = min / 60;
	d = hours / 24;
	sec = fmod(sec, 60.0F);
	if (d > 0)
		sprintf(s, "Time: %u d %u h %u m %.2f s", d, hours % 24, min % 60, sec);
	else if (hours > 0)
		sprintf(s, "Time: %u h %u m %.2f s", hours, min % 60, sec);
	else if (min > 0)
		sprintf(s, "Time: %u m %.2f s", min, sec);
	else
		sprintf(s, "Time: %.2f s", sec);
	sgl_draw_str(s, &(t_font){"SourceSans", 24, 0xFFFFFF, 0},
					(t_point){10, 45, 0}, rend);
}

void	display_stats(t_env *env, SDL_Renderer *rend)
{
	char	s[512];

	display_fps(rend);
	display_render_time(env, rend);
	sprintf(s, "Samples: %u", env->scene.sampls);
	sgl_draw_str(s, &(t_font){"SourceSans", 24, 0xFFFFFF, 0},
					(t_point){10, 25, 0}, rend);
}
