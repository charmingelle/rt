#include "rt.h"

char	*get_name(char *string)
{
	int		i;
	char	*name;

	i = 0;
	while (string[i] && string[i] != ':')
		i++;
	return (ft_strsub(string, 0, i));
}

char	*get_value(char *string)
{
	int		i;
	int		len;

	i = 0;
	while (string[i] && string[i] != ':')
		i++;
	if (string[i] == 0)
		return (NULL);
	i++;
	len = ft_strlen(string);
	return (ft_strsub(string, i, len - i));
}

void	skip_comma(char **string)
{
	if (**string != ',' && **string != '}' && **string != ']')
		ft_err_handler("Comma is missing", 0, 0, 1);
	if (**string == ',')
		(*string)++;
}

void	skip_open_brace(char **string)
{
	if (**string != '{')
		ft_err_handler("Open brace is missing", 0, 0, 1);
	(*string)++;
}

void	skip_close_brace(char **string)
{
	if (**string != '}')
		ft_err_handler("Closing brace is missing", 0, 0, 1);
	(*string)++;
}

void	skip_open_bracket(char **string)
{
	if (**string != '[')
		ft_err_handler("Open bracket is missing", 0, 0, 1);
	(*string)++;
}

void	skip_close_bracket(char **string)
{
	if (**string != ']')
		ft_err_handler("Closing bracket is missing", 0, 0, 1);
	(*string)++;
}

int		is_digit(char c)
{
	return ((c >= '0' && c <= '9') || c == '-');
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

int		get_primitive_end(char *string)
{
	int	i;

	i = 0;
	while (string[i] && string[i] != ',' && string[i] != '}')
		i++;
	if (string[i] == 0)
		return (-1);
	return (i);
}

char	*parse_u_int(char *string, t_uint *place_to_save)
{
	int		end;
	char	*head;
	int		value;

	end = get_primitive_end(string);
	head = ft_strsub(string, 0, end);
	if ((value = ft_atoi(head)) < 0)
		ft_err_handler("Camera distance cannot be negative", 0, 0, 1);
	*place_to_save = value;
	free(head);
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

char	*parse_int(char *string, int *place_to_save)
{
	int		end;
	char	*head;

	end = get_primitive_end(string);
	head = ft_strsub(string, 0, end);
	*place_to_save = ft_atoi(head);
	free(head);
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

char	*parse_float(char *string, float *place_to_save)
{
	int		end;
	char	*head;	

	end = get_primitive_end(string);
	head = ft_strsub(string, 0, end);
	*place_to_save = ft_atof(head);
	free(head);
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

char	*parse_ufloat(char *string, float *place_to_save)
{
	int		end;
	char	*head;
	float	value;

	end = get_primitive_end(string);
	head = ft_strsub(string, 0, end);
	if ((value = ft_atof(head)) < 0)
		ft_err_handler("Figure radius, angle, reflection, transparency; light power cannot be negative", 0, 0, 1);
	*place_to_save = value;
	free(head);
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

char	*parse_string(char *string, char **place_to_save)
{
	int		end;

	end = get_primitive_end(string);
	if (string[0] != '"' || string[end - 1] != '"')
		ft_err_handler("String values should be in quote marks", 0, 0, 1);
	*place_to_save = ft_strsub(string, 1, end - 2);
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

int		get_figure_type(char *string)
{
	char	*types[4] = {"\"sphere\"", "\"cylinder\"", "\"cone\"", "\"plane\""};
	int		i;

	i = 0;
	while (i < 4)
	{
		if (!ft_strcmp(string, types[i]))
			return (i);
		i++;
	}
	ft_err_handler("Unknown figure type", 0, 0, 1);
	return (-1);
}

char	*parse_figure_type(char *string, short int *place_to_save)
{
	int		end;

	end = get_primitive_end(string);
	*place_to_save = get_figure_type(ft_strsub(string, 0, end));
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

char	*parse_cl_float3(char *string, cl_float3 *place_to_save)
{
	char	*name;
	char	*value;
	
	skip_open_brace(&string);
	while (*string && *string != '}')
	{
		name = get_name(string);
		value = get_value(string);
		if (!ft_strcmp(name, "\"x\""))
			string = parse_float(value, &place_to_save->x);
		else if (!ft_strcmp(name, "\"y\""))
			string = parse_float(value, &place_to_save->y);
		else if (!ft_strcmp(name, "\"z\""))
			string = parse_float(value, &place_to_save->z);
		else
			ft_err_handler("Unknown point property", 0, 0, 1);
		skip_comma(&string);
		free(name);
		free(value);
	}
	skip_close_brace(&string);
	return (string);
}

char	*parse_camera(char *string, t_cam *place_to_save)
{
	char	*name;
	char	*value;

	skip_open_brace(&string);
	while (*string && *string != '}')
	{
		name = get_name(string);
		value = get_value(string);
		if (!ft_strcmp(name, "\"position\""))
			string = parse_cl_float3(value, &place_to_save->pos);
		else if (!ft_strcmp(name, "\"angles\""))
			string = parse_cl_float3(value, &place_to_save->rot);
		else if (!ft_strcmp(name, "\"distance\""))
			string = parse_u_int(value, &place_to_save->rot_os);
		else
			ft_err_handler("Unknown camera property", 0, 0, 1);
		skip_comma(&string);
		free(name);
		free(value);
	}
	skip_close_brace(&string);
	return (string);
}

char	*parse_figure(char *string, t_obj *place_to_save)
{
	char	*name;
	char	*value;

	skip_open_brace(&string);
	while (*string && *string != '}') {
		name = get_name(string);
		value = get_value(string);
		if (!ft_strcmp(name, "\"type\""))
			string = parse_figure_type(value, &place_to_save->type);
		else if (!ft_strcmp(name, "\"center\""))
			string = parse_cl_float3(value, &place_to_save->pos);
		else if (!ft_strcmp(name, "\"center2\"") || !ft_strcmp(name, "\"normal\""))
			string = parse_cl_float3(value, &place_to_save->dir);
		else if (!ft_strcmp(name, "\"emission\""))
			string = parse_cl_float3(value, &place_to_save->emission);
		else if (!ft_strcmp(name, "\"radius\""))
			string = parse_ufloat(value, &place_to_save->rad);
		else if (!ft_strcmp(name, "\"angle\""))
			string = parse_ufloat(value, &place_to_save->rad);
		else if (!ft_strcmp(name, "\"color\""))
			string = parse_cl_float3(value, &place_to_save->color);
		else if (!ft_strcmp(name, "\"shine\""))
			string = parse_int(value, &place_to_save->spec);
		else if (!ft_strcmp(name, "\"reflection\""))
			string = parse_ufloat(value, &place_to_save->refl);
		else if (!ft_strcmp(name, "\"transparency\""))
			string = parse_ufloat(value, &place_to_save->transp);
		else if (!ft_strcmp(name, "\"texture\""))
			string = parse_string(value, &place_to_save->text);
		else
			ft_err_handler("Unknown figure property", 0, 0, 1);
		skip_comma(&string);
		free(name);
		free(value);
	}
	skip_close_brace(&string);
	return (string);
}

int		count_objects(char *string)
{
	int	count;
	int	end;

	count = 0;
	while (*string && *string != ']')
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

	place_to_save->objs_c = count_objects(string);
	!(place_to_save->objs_h = malloc(sizeof(t_obj) * (place_to_save->objs_c + 1)))
		? ft_err_handler("malloc", "can't allocate region!", 0, 1) : 0;
	i = 0;
	skip_open_bracket(&string);
	while (i < place_to_save->objs_c)
	{
		string = parse_figure(string, &place_to_save->objs_h[i]);
		skip_comma(&string);
		i++;
	}
	skip_close_bracket(&string);
	place_to_save->objs_h[place_to_save->objs_c].type = -1;
	return (string);
}

char	get_light_type(char *string)
{
	if (!ft_strcmp(string, "\"ambient\""))
		return (0);
	if (!ft_strcmp(string, "\"point\""))
		return (1);
	ft_err_handler("Unknown light type!", 0, 0, 1);
	return (2);
}

char	*parse_light_type(char *string, char *place_to_save)
{
	int		end;

	end = get_primitive_end(string);
	*place_to_save = get_light_type(ft_strsub(string, 0, end));
	return (ft_strsub(string, end, ft_strlen(string) - end));
}

char	*parse_light(char *string, t_light *place_to_save)
{
	char	*name;
	char	*value;

	skip_open_brace(&string);
	while (*string && *string != '}') {
		name = get_name(string);
		value = get_value(string);
		if (!ft_strcmp(name, "\"type\""))
			string = parse_light_type(value, &place_to_save->type);
		else if (!ft_strcmp(name, "\"power\""))
			string = parse_ufloat(value, &place_to_save->intens);
		else if (!ft_strcmp(name, "\"position\""))
			string = parse_cl_float3(value, &place_to_save->pos);
		else
			ft_err_handler("Unknown light property", 0, 0, 1);
		skip_comma(&string);
		free(name);
		free(value);
	}
	skip_close_brace(&string);
	return (string);
}

char	*parse_light_array(char *string, t_scene *place_to_save)
{
	int	i;

	place_to_save->light_c = count_objects(string);
	!(place_to_save->light_h = malloc(sizeof(t_light) * (place_to_save->light_c + 1)))
		? ft_err_handler("malloc", "can't allocate region!", 0, 1) : 0;
	i = 0;
	skip_open_bracket(&string);
	while (i < place_to_save->light_c)
	{
		string = parse_light(string, &place_to_save->light_h[i]);
		i++;
		skip_comma(&string);
	}
	skip_close_bracket(&string);
	place_to_save->light_h[place_to_save->light_c].type = -1;
	return (string);
}

void	parse_scene(char *string, t_env *env)
{
	char	*name;
	char	*value;
	
	skip_open_brace(&string);
	while (*string && *string != '}')
	{
		name = get_name(string);
		value = get_value(string);
		if (!ft_strcmp(name, "\"camera\""))
			string = parse_camera(value, env->cam);
		else if (!ft_strcmp(name, "\"figures\""))
			string = parse_figure_array(value, &env->scene);
		else if (!ft_strcmp(name, "\"lights\""))
			string = parse_light_array(value, &env->scene);
		else
			ft_err_handler("Unknown scene property", 0, 0, 1);
		skip_comma(&string);
		free(name);
		free(value);
	}
	skip_close_brace(&string);
}

int		count_whitespaces(char *string)
{
	int	quote;
	int	count;

	quote = 0;
	count = 0;
	while (*string) {
		if (*string == '"')
			quote ^= 1;
		if ((*string == ' ' || *string == '\t' || *string == '\n') && quote)
			count++;
		string++;
	}
	return (count);
}

char	*remove_whitespaces(char *string)
{
	char	*result;
	int		quote;
	int		i;

	result = (char *)malloc(ft_strlen(string) - count_whitespaces(string) + 1);
	quote = 0;
	i = 0;
	while (*string)
	{
		if (*string == '"')
			quote ^= 1;
		if ((*string != ' ' && *string != '\t' && *string != '\n') || quote) {
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
	parse_scene(remove_whitespaces(result), env);
}
