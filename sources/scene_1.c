/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/14 17:36:47 by pgritsen          #+#    #+#             */
/*   Updated: 2018/05/15 19:22:06 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			get_primitive_end(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ',' && str[i] != '}' && str[i] != ']')
		i++;
	if (str[i] == 0)
		return (-1);
	return (i);
}

char		*remove_whitespaces(char *string)
{
	char	*result;
	int		quote;
	int		i;

	result = (char *)ft_memalloc(ft_strlen(string) + 1);
	quote = 0;
	i = 0;
	while (*string)
	{
		if (*string == '"')
			quote ^= 1;
		if ((*string != ' ' && *string != '\t' && *string != '\n') || quote)
		{
			result[i] = *string;
			i++;
		}
		string++;
	}
	result[i] = 0;
	return (result);
}

char		*get_file_content(char *name)
{
	int		fd;
	char	*line;
	char	*temp;
	char	*result;

	fd = open(name, O_RDONLY);
	if (fd == -1)
		ft_err_handler("No such file", 0, 0, 1);
	result = NULL;
	while (ft_get_next_line(fd, &line) == 1)
	{
		temp = result;
		result = ft_strjoin(result, line);
		free(temp);
		free(line);
	}
	free(line);
	return (result);
}

void		read_scene(char *name, int ac, t_env *env)
{
	char	*result;
	char	*no_whitespaces;
	char	*no_whitespaces_start;

	ac != 2 ? display_usage(0) : 0;
	!ft_strcmp("-help", name) ? display_usage(1) : 0;
	ft_printf("Scene {yellow}'%s'{nc} parsing started...", name);
	result = get_file_content(name);
	no_whitespaces = remove_whitespaces(result);
	no_whitespaces_start = no_whitespaces;
	free(result);
	parse_scene(&no_whitespaces, env);
	free(no_whitespaces_start);
	ft_printf("{green}OK{nc}\n");
}
