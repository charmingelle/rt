/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 22:32:46 by pgritsen          #+#    #+#             */
/*   Updated: 2017/11/10 20:59:04 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return ((unsigned char)c - ('a' - 'A'));
	return (c);
}
