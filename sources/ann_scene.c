#include "rtv1.h"

char	*get_name(char *string)
{
	ft_printf("GET_NAME\n");
	int		i;
	char	*name;

	i = 0;
	while (string[i] && string[i] != ':')
		i++;
	if (string[i] == 0)
		return (NULL);
	ft_printf("GET_NAME_END\n");
	return (ft_strsub(string, 0, i));
}

char	*get_value(char *string)
{
	ft_printf("GET_VALUE\n");
	int		i;
	int		len;

	i = 0;
	while (string[i] && string[i] != ':')
		i++;
	if (string[i] == 0)
		return (NULL);
	i++;
	len = ft_strlen(string);
	ft_printf("GET_VALUE_END\n");	
	return (ft_strsub(string, i, len - i));
}

void	skip_extra(char **string)
{
	while (*string && (**string == ',' || **string == '{' || **string == '[' || **string == '}' || **string == ']'))
		(*string)++;
}

// need to pass address afrer { or []} as str. AFTER?

char	*check_brackets(char *str, char sc)
{
	char	tmp;

	while (*str != sc)
		if (*str == '{' || *str == '[' || *str == '"')
		{
			tmp = *str++;
			if (!(str = check_brackets(str, tmp + 2)))
				return (NULL);
		}
		else if (*str == '}' || *str == ']' || !*str || *str == '"')
			return (NULL);
		else
			str++;
	return (str + 1);
}

// returns address after } or ]

int		count_up_to(char *string, char *end_address)
{
	int	amount;

	amount = 0;
	while (*string && string != end_address) {
		amount++;
		string++;
	}
	return (amount);
}

int		is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int		get_object_end(char *string)
{
	int	i;
	int	open_brackets;

	i = 0;
	open_brackets = 0;
	while (string[i])
	{
		if (string[i] == '{')
			open_brackets++;
		if (string[i] == '}')
			open_brackets--;
		if (string[i] == '}' && open_brackets == 0)
		{
			i++;
			break;
		}
		i++;
	}
	if (string[i] == 0)
		return (-1);
	return (i);
}

int		get_int_end(char *string)
{
	int	i;

	i = 0;
	while (string[i] && is_digit(string[i]))
		i++;
	if (string[i] == 0)
		return (-1);
	return (i);
}

char	*parse_int(char *string, t_uint *place_to_save)
{
	int		end;
	char	*head;

	end = get_int_end(string);
	head = ft_strsub(string, 0, end);
	*place_to_save = ft_atoi(head);
	free(head);
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

int		get_double_end(char *string) {
	int	i;

	i = 0;
	while (string[i] && (is_digit(string[i]) || string[i] == '.'))
		i++;
	if (string[i] == 0)
		return (-1);
	return (i);
}

char	*parse_double(char *string, double *place_to_save)
{
	int		end;
	char	*head;	

	end = get_double_end(string);
	head = ft_strsub(string, 0, end);
	ft_printf("place_to_save = %p, double = %f\n", place_to_save, ft_atof(head));
	*place_to_save = ft_atof(head);
	free(head);
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

int		get_string_end(char *string)
{
	int	i;

	i = 0;
	while (string[i] && string[i] != '"')
		i++;
	if (string[i] == 0)
		return (-1);
	return (i);
}

char	*parse_string(char *string, char **place_to_save)
{
	int		end;

	end = get_string_end(string);
	*place_to_save = ft_strsub(string, 0, end);
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

int		get_type(char *string)
{
	char	*types[4] = {"sphere", "cylinder", "cone", "plane"};
	int		i;

	i = 0;
	while (i < 4)
	{
		if (!ft_strcmp(string, types[i]))
			return (i);
		i++;
	}
	return (-1);
}

char	*parse_type(char *string, int *place_to_save)
{
	int		end;

	end = get_string_end(string);
	*place_to_save = get_type(ft_strsub(string, 0, end));
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

char	*parse_point(char *string, t_point *place_to_save)
{
	char	*name;
	char	*value;
	char	*end_address;
	int		len;
	
	while (*string)
	{
		if (*string == '}')
		{
			skip_extra(&string);
			break;
		}
		skip_extra(&string);
		name = get_name(string);
		value = get_value(string);
		ft_printf("name = %s\n", name);
		ft_printf("value = %s\n", value);
		if (!ft_strcmp(name, "\"x\""))
		{
			ft_printf("x = %p\n", &place_to_save->x);
			string = parse_double(value, &place_to_save->x);
		}
		else if (!ft_strcmp(name, "\"y\""))
			string = parse_double(value, &place_to_save->y);
		else if (!ft_strcmp(name, "\"z\""))
			string = parse_double(value, &place_to_save->z);
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		free(name);
		free(value);
	}
	return (string);
}

char	*parse_rotate(char *string, t_rotate *place_to_save)
{
	char	*name;
	char	*value;

	while (*string)
	{
		if (*string == '}')
		{
			skip_extra(&string);
			break;
		}
		skip_extra(&string);
		name = get_name(string);
		value = get_value(string);
		ft_printf("name = %s\n", name);
		ft_printf("value = %s\n", value);
		if (!ft_strcmp(name, "\"rx\""))
			string = parse_double(value, &place_to_save->rx);
		else if (!ft_strcmp(name, "\"ry\""))
			string = parse_double(value, &place_to_save->ry);
		else if (!ft_strcmp(name, "\"rz\""))
			string = parse_double(value, &place_to_save->rz);
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		free(name);
		free(value);
	}
	return (string);
}

char	*parse_camera(char *string, t_cam *place_to_save)
{
	char	*name;
	char	*value;

	while (*string)
	{
		if (*string == '}')
		{
			skip_extra(&string);
			break;
		}
		skip_extra(&string);
		name = get_name(string);
		value = get_value(string);
		ft_printf("FROM CAMERA: string = %s\n", string);
		ft_printf("name = %s\n", name);
		ft_printf("compare result = %d\n", ft_strcmp(name, "\"angles\""));
		ft_printf("value = %s\n", value);
		if (!ft_strcmp(name, "\"position\""))
		{
			ft_printf("cam = %p,\npos = %p\n", place_to_save, &(place_to_save->pos));
			string = parse_point(value, &place_to_save->pos);
		}
		else if (!ft_strcmp(name, "\"angles\""))
			string = parse_rotate(value, &place_to_save->rot);
		else if (!ft_strcmp(name, "\"distance\""))
			string = parse_int(value, &place_to_save->rot_os);
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		free(name);
		free(value);
	}
	return (string);
}

// env->scene.objs_h[index].type = ABS(ft_atoi(arr[0]));
// env->scene.objs_h[index].color = ft_atol_base(arr[1], 16);
// env->scene.objs_h[index].pos = sgl_atop(arr[2]);
// env->scene.objs_h[index].dir = sgl_atop(arr[3]);
// if (!sgl_check_point(env->scene.objs_h[index].pos)
// 	|| !sgl_check_point(env->scene.objs_h[index].dir))
// 	ft_err_handler("Object broken!", 0, 0, 1);
// env->scene.objs_h[index].rad = ABS(ft_atof(arr[4]));
// env->scene.objs_h[index].spec = ABS(ft_atoi(arr[5]));
// env->scene.objs_h[index].refl = ABS(ft_atof(arr[6]));

char	*parse_figure(char *string, t_obj *place_to_save)
{
	char	*name;
	char	*value;

	while (*string) {
		if (*string == '}')
		{
			skip_extra(&string);
			break;
		}
		skip_extra(&string);
		name = get_name(string);
		value = get_value(string);
		ft_printf("name = %s\n", name);
		ft_printf("value = %s\n", value);
		if (!ft_strcmp(name, "\"type\""))
			string = parse_type(value, &place_to_save->type);
		else if (!ft_strcmp(name, "\"center\""))
			string = parse_point(value, &place_to_save->pos);
		else if (!ft_strcmp(name, "\"center2\"") || !ft_strcmp(name, "\"normal\""))
			string = parse_point(value, &place_to_save->dir);
		else if (!ft_strcmp(name, "\"radius\""))
			string = parse_double(value, &place_to_save->rad);
		else if (!ft_strcmp(name, "\"color\""))
			string = parse_point(value, &place_to_save->color);
		else if (!ft_strcmp(name, "\"shine\""))
			string = parse_double(value, &place_to_save->spec);
		else if (!ft_strcmp(name, "\"reflection\""))
			string = parse_double(value, &place_to_save->refl);
		// else if (!ft_strcmp(name, "\"transparency\""))
		// 	string = parse_double(value, &place_to_save.transp);
		// else if (!ft_strcmp(name, "\"texture\""))
		// 	string = parse_texture(value, &place_to_save.texture);
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		skip_extra(&string);
	}
	// validate_figure(figure, type);
	return (string);
}

int		count_objects(char *string)
{
	int	count;
	int	end;

	count = 0;
	while (*string)
	{
		if ((end = get_object_end(string)) == -1)
			break ;
		string = ft_strsub(string, end, ft_strlen(string) - end);
		count++;
	}
	return (count);
}

char	*parse_figure_array(char *string, t_scene *place_to_save)
{
	int	i;
	int	end;

	place_to_save->objs_c = count_objects(string);
	!(place_to_save->objs_h = malloc(sizeof(t_obj) * (place_to_save->objs_c + 1)))
		? ft_err_handler("malloc", "can't allocate region!", 0, 1) : 0;
	i = 0;
	while (*string)
	{
		if (*string == ']')
		{
			skip_extra(&string);
			break;
		}
		end = get_object_end(string);
		string = parse_figure(ft_strsub(string, 0, end), &place_to_save->objs_h[i]);
		string = ft_strsub(string, end, ft_strlen(string) - end);
		i++;
	}
	return (string);
}

char	*parse_scene(char *string, t_env *place_to_save)
{
	char	*name;
	char	*value;
	
	while (*string)
	{
		if (*string == '}')
		{
			skip_extra(&string);
			break;
		}
		skip_extra(&string);
		name = get_name(string);
		value = get_value(string);
		ft_printf("name = %s\n", name);
		ft_printf("value = %s\n", value);
		if (!ft_strcmp(name, "\"camera\""))
		{
			ft_printf("env = %p,\ncam = %p\n", place_to_save, place_to_save->cam);
			string = parse_camera(value, place_to_save->cam);
		}
		else if (!ft_strcmp(name, "\"figures\""))
			string = parse_figure_array(value, &place_to_save->scene);
		// else if (!ft_strcmp(name_name_value[0], "lights"))
		// 	string = parse_light(value, place_to_save->scene->light_h);
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		free(name);
		free(value);
	}
	return (string);
}

char	*parse_light(char *string, void *place_to_save)
{
// 	char	**name_name;

// 	while (*string) {
// 		name_name = ft_strsplit(string, ":");
// 		if (count_split(name_name) != 2)
// 			ft_err_handler("Scene broken!", 0, 0, 1);
// 		if (!ft_strcmp(name_name[0], "type")))
// 			string = get_object_or_array_string(name_value[1], "string", place_to_save);
// 		else if (!ft_strcmp(name_name[0], "power")))
// 			string = get_object_or_array_string(name_value[1], "double", place_to_save);
// 		else if (!ft_strcmp(name_name[0], "position"))
// 			string = get_object_or_array_string(name_value[1], "point", place_to_save);
// 		else if (!ft_strcmp(name_name[0], "direction"))
// 			string = get_object_or_array_string(name_value[1], "point", place_to_save);
// 		else
// 			ft_err_handler("Scene broken!", 0, 0, 1);
// 		skip_extra(string);
// 	}
// 	validate_light(figure, type);
	return NULL;
}

void	parse(char *string, t_env *env)
{
	if (string[0] != '{')
		ft_err_handler("Scene broken!", 0, 0, 1);
	ft_printf("env = %p\n", env);
	parse_scene(string, env);
}

int		count_whitespaces(char *string)
{
	int	count;

	count = 0;
	while (*string) {
		if (*string == ' ' || *string == '\t' || *string == '\n')
			count++;
		string++;
	}
	return (count);
}

char	*remove_whitespaces(char *string)
{
	char	*result;
	int		i;

	result = (char *)malloc(ft_strlen(string) - count_whitespaces(string) + 1);
	i = 0;
	while (*string)
	{
		if (*string != ' ' && *string != '\t' && *string != '\n') {
			result[i] = *string;
			i++;
		}
		string++;
	}
	result[i] = 0;
	return (result);
}

void	read_scene(char *name, t_env *env)
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
	parse(remove_whitespaces(result), env);
}
