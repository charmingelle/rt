#include "rtv1.h"

#define SCENE 0
#define CAMERA 1
#define FIGURE 2
#define LIGHT 3
#define POINT 4
#define STRING 5
#define FLOAT 6
#define ARRAY 7
#define ROTATE 8

char	*get_name(char *string)
{
	int		i;
	char	*name;

	i = 0;
	while (string[i] && string[i] != ':')
		i++;
	if (string[i] == 0)
		return (NULL);
	name = (char *)malloc(i + 1);
	i = 0;
	while (string[i] != ':')
	{
		name[i] = string[i];
		i++;
	}
	name[i] = 0;
	return (name);
}

char	*get_value(char *string)
{
	int		i;
	int		len;
	char	*value;
	int		j;

	i = 0;
	while (string[i] && string[i] != ':')
		i++;
	if (string[i] == 0)
		return (NULL);
	i++;
	len = ft_strlen(string);
	value = (char *)malloc(len - i + 1);
	j = 0;
	while (string[i])
		value[j++] = string[i++];
	value[j] = 0;
	return (value);
}

void	skip_extra(char **string)
{
	if (**string == ',' || **string == '}' || **string == ']')
		(*string)++;
}

// need to pass address afrer { or []} as str. AFTER?

char	*check_brackets(char *str, char sc)
{
	char	tmp;

	while (*str != sc)
		if (*str == '{' || *str == '[')
		{
			tmp = *str++;
			if (!(str = check_brackets(str, tmp + 2)))
				return (NULL);
		}
		else if (*str == '}' || *str == ']' || !*str)
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

void	parse_float(char *string, void *place_to_save)
{
	*((float *)place_to_save) = ft_atof(string);
}

void	parse_point(char *string, void *place_to_save)
{
	char	*name;
	char	*value;

	while (*string)
	{
		name = get_name(string);
		value = get_value(string);
		ft_printf("%s\n", name);
		if (!ft_strcmp(name, "x"))
			string = get_object_or_array_string(value, FLOAT, &(((t_point *)place_to_save)->x));
		else if (!ft_strcmp(name, "y"))
			string = get_object_or_array_string(value, FLOAT, &(((t_point *)place_to_save)->y));
		else if (!ft_strcmp(name, "z"))
			string = get_object_or_array_string(value, FLOAT, &(((t_point *)place_to_save)->z));
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		skip_extra(&string);
	}
}

void	parse_rotate(char *string, void *place_to_save)
{
	char	*name;
	char	*value;

	while (*string)
	{
		name = get_name(string);
		value = get_value(string);	
		if (!ft_strcmp(name, "x"))
			string = get_object_or_array_string(value, FLOAT, &(((t_rotate *)place_to_save)->rx));
		else if (!ft_strcmp(name, "y"))
			string = get_object_or_array_string(value, FLOAT, &(((t_rotate *)place_to_save)->ry));
		else if (!ft_strcmp(name, "z"))
			string = get_object_or_array_string(value, FLOAT, &(((t_rotate *)place_to_save)->rz));
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		skip_extra(&string);
	}
}

void	parse_string(char *string, void *place_to_save)
{
	*((char **)place_to_save) = string;
}

void	parse_camera(char *string, void *place_to_save)
{
	char	*name;
	char	*value;

	while (*string)
	{
		name = get_name(string);
		value = get_value(string);
		ft_printf("%s\n", name);
		if (!ft_strcmp(name, "position"))
			string = get_object_or_array_string(value, POINT, &(((t_cam *)place_to_save)->pos));
		else if (!ft_strcmp(name, "angles"))
			string = get_object_or_array_string(value, ROTATE, &(((t_cam *)place_to_save)->rot));
		else if (!ft_strcmp(name, "distance"))
			string = get_object_or_array_string(value, FLOAT, &(((t_cam *)place_to_save)->rot_os));
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		skip_extra(&string);
	}
}

void	parse_scene(char *string, void *place_to_save)
{
	char	*name;
	char	*value;

	while (*string)
	{
		name = get_name(string);
		value = get_value(string);
		ft_printf("%s\n", name);
		if (!ft_strcmp(name, "camera"))
			string = get_object_or_array_string(value, CAMERA, &(((t_env *)place_to_save)->cam));
		// else if (!ft_strcmp(name_name_value[0], "figures"))
		// 	string = get_object_or_array_string(name_value[1], FIGURE, place_to_save->scene->objs_h);
		// else if (!ft_strcmp(name_name_value[0], "lights"))
		// 	string = get_object_or_array_string(name_value[1], LIGHT, place_to_save->scene->light_h);
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
		skip_extra(&string);
	}
}

void	parse_array(char *string, void *place_to_save)
{
// 	char	**name_name;

// 	while (*string)
// 	{
// 		string = get_object_or_array_string(string, type, place_to_save)
// 		skip_extra(string);
// 	}
}

void	parse_figure(char *string, void *place_to_save)
{
// 	char	**name_name;

// 	while (*string) {
// 		name_name = ft_strsplit(string, ":");
// 		if (count_split(name_name) != 2)
// 			ft_err_handler("Scene broken!", 0, 0, 1);
// 		if (!ft_strcmp(name_value[0], "type")))
// 			string = get_object_or_array_string(name_value[1], "string", place_to_save);
// 		else if (!ft_strcmp(name_value[0], "center") || !ft_strcmp(name_name_value[1][0], "center2"))
// 			string = get_object_or_array_string(name_value[1], "point", place_to_save);
// 		else if (!ft_strcmp(name_value[0], "radius") || !ft_strcmp(name_name_value[1][0], "shine") || !ft_strcmp(name_name_value[1][0], "reflection") || !ft_strcmp(name_name_value[1][0], "transparency"))
// 			string = get_object_sring(name_value[1], "float", place_to_save);
// 		else if (!ft_strcmp(name_value[0], "color"))
// 			string = get_color(name_value[1], place_to_save);
// 		else if (!ft_strcmp(name_value[0], "texture"))
// 			string = get_texture(name_value[1], place_to_save);
// 		else
// 			ft_err_handler("Scene broken!", 0, 0, 1);
// 		skip_extra(string);
// 	}
// 	validate_figure(figure, type);
}

void	parse_light(char *string, void *place_to_save)
{
// 	char	**name_name;

// 	while (*string) {
// 		name_name = ft_strsplit(string, ":");
// 		if (count_split(name_name) != 2)
// 			ft_err_handler("Scene broken!", 0, 0, 1);
// 		if (!ft_strcmp(name_name[0], "type")))
// 			string = get_object_or_array_string(name_value[1], "string", place_to_save);
// 		else if (!ft_strcmp(name_name[0], "power")))
// 			string = get_object_or_array_string(name_value[1], "float", place_to_save);
// 		else if (!ft_strcmp(name_name[0], "position"))
// 			string = get_object_or_array_string(name_value[1], "point", place_to_save);
// 		else if (!ft_strcmp(name_name[0], "direction"))
// 			string = get_object_or_array_string(name_value[1], "point", place_to_save);
// 		else
// 			ft_err_handler("Scene broken!", 0, 0, 1);
// 		skip_extra(string);
// 	}
// 	validate_light(figure, type);
}

void	parse_text_as(char *string, int type, void *place_to_save)
{
	void	(*parsers[9]) (char *text, void *place_to_save);

	parsers[0] = parse_scene;
	parsers[1] = parse_camera;
	parsers[2] = parse_figure;
	parsers[3] = parse_light;
	parsers[4] = parse_point;
	parsers[5] = parse_string;
	parsers[6] = parse_float;
	parsers[7] = parse_array;
	parsers[8] = parse_rotate;
	(*parsers[type])(string, place_to_save);
}

char	*get_object_or_array_string(char *string, int type, void *place_to_save)
{
	char	*end_address;
	int		len;

	end_address = check_brackets(string, 0);
	len = count_up_to(string, end_address);
	string++;
	parse_text_as(ft_strsub(string, 0, len), type, place_to_save);
	return (end_address);
}

void	parse(char *string, t_env *env)
{
	if (string[0] != '{')
		ft_err_handler("Scene broken!", 0, 0, 1);
	get_object_or_array_string(string, SCENE, env);
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
	int		whitespaces;
	char	*result;
	int		i;

	whitespaces = count_whitespaces(string);
	result = (char *)malloc(whitespaces + 1);
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
