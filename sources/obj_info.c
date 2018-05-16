/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/15 18:41:18 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/15 19:49:22 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		draw_info(SDL_Rect *p, SDL_Renderer *rend)
{
	p->w = 200;
	p->h = 200;
	SDL_SetRenderDrawColor(rend, 0xC0, 0xC0, 0xC0, 0xFF);
	SDL_RenderFillRect(rend, p);
	SDL_SetRenderDrawColor(rend, 0xAA, 0xAA, 0xAA, 0xFF);
	SDL_RenderFillRect(rend, &(SDL_Rect){p->x + 3, p->y + 3, p->w - 6, p->h - 6});
}
