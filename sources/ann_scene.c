#include "rtv1.h"

int		count_split(char **split) {
	int	amount;

	amount = 0;
	while (*split++)
		amount++;
	return (amount);
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

int		count_up_to(char *string, char *end_address) {
	int	amount;

	amount = 0;
	while (*string && string != end_address) {
		amount++;
		string++;
	}
	return (amount);
}

void	skip_commas_and_whitespaces(char *string) {
	while (*string && (*string == ',' || *string == ' ' || *string == '\n' || *string == '\t'))
		string++;
}

void	parse_scene_object(char *string, void *place_to_save) {
	while (*string) {
		split = ft_strsplit(string, ":");
		if (count_split(split) != 2)
			ft_err_handler("Scene broken!", 0, 0, 1);
		name = split[0];
		value = split[1];
		if (!ft_strcmp(name, "camera")))
			string = skip_commas_and_whitespaces(get_object_string(value, "camera", place_to_save->cam));
		else if (!ft_strcmp(name, "figures"))
			string = skip_commas_and_whitespaces(get_array_string(value, place_to_save->scene->objs_h));
		else if (!ft_strcmp(name, "lights"))
			string = skip_commas_and_whitespaces(get_array_string(value, place_to_save->scene->light_h));
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
	}
}

void	parse_camera_object(char *string, void *place_to_save) {
	while (*string) {
		split = ft_strsplit(string, ":");
		if (count_split(split) != 2)
			ft_err_handler("Scene broken!", 0, 0, 1);
		name = split[0];
		value = split[1];
		if (!ft_strcmp(name, "position")))
			string = skip_commas_and_whitespaces(get_object_string(value, "point", place_to_save));
		else if (!ft_strcmp(name, "angles"))
			string = skip_commas_and_whitespaces(get_object_string(value, "point", place_to_save));
		else if (!ft_strcmp(name, "distance"))
			string = skip_commas_and_whitespaces(get_object_string(value, "float", place_to_save));
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
	}
}

void	parse_figure_object(char *string, void *place_to_save) {
	while (*string) {
		split = ft_strsplit(string, ":");
		if (count_split(split) != 2)
			ft_err_handler("Scene broken!", 0, 0, 1);
		name = split[0];
		value = split[1];
		if (!ft_strcmp(name, "type")))
			string = skip_commas_and_whitespaces(get_object_string(value, "string", place_to_save));
		else if (!ft_strcmp(name, "center") || !ft_strcmp(name, "center2"))
			string = skip_commas_and_whitespaces(get_object_string(value, "point", place_to_save));
		else if (!ft_strcmp(name, "radius") || !ft_strcmp(name, "shine") || !ft_strcmp(name, "reflection") || !ft_strcmp(name, "transparency"))
			string = skip_commas_and_whitespaces(get_object_sring(value, "float", place_to_save));
		else if (!ft_strcmp(name, "color"))
			string = skip_commas_and_whitespaces(get_color(value, place_to_save));
		else if (!ft_strcmp(name, "texture"))
			string = skip_commas_and_whitespaces(get_texture(value, place_to_save));
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
	}
	validate_figure(figure, type);
}

void	parse_light_object(char *string, void *place_to_save) {
	while (*string) {
		split = ft_strsplit(string, ":");
		if (count_split(split) != 2)
			ft_err_handler("Scene broken!", 0, 0, 1);
		name = split[0];
		value = split[1];
		if (!ft_strcmp(name, "type")))
			string = skip_commas_and_whitespaces(get_object_string(value, "string", place_to_save));
		else if (!ft_strcmp(name, "power")))
			string = skip_commas_and_whitespaces(get_object_string(value, "float", place_to_save));
		else if (!ft_strcmp(name, "position"))
			string = skip_commas_and_whitespaces(get_object_string(value, "point", place_to_save));
		else if (!ft_strcmp(name, "direction"))
			string = skip_commas_and_whitespaces(get_object_string(value, "point", place_to_save));
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
	}
	validate_light(figure, type);
}

void	parse_point_object(char *string, void *place_to_save) {
	while (*string) {
		split = ft_strsplit(string, ":");
		if (count_split(split) != 2)
			ft_err_handler("Scene broken!", 0, 0, 1);
		name = split[0];
		value = split[1];
		if (!ft_strcmp(name, "x")) || ft_strcmp(name, "y") || !ft_strcmp(name, "z"))
			string = skip_commas_and_whitespaces(get_object_string(value, "float", place_to_save));
		else
			ft_err_handler("Scene broken!", 0, 0, 1);
	}
}

void	parse_object(char *string, char *type, void *place_to_save) {
	char	**split;
	char	*name;
	char	*value;
	char	*tail_start;

	split = ft_strsplit(string, ":");
	if (count_split(split) != 2)
		ft_err_handler("Scene broken!", 0, 0, 1);
	name = split[0];
	value = split[1];
	if (!ft_strcmp(name, "scene"))
		parse_scene_object(value, place_to_save);
	else if (!ft_strcmp(name, "camera"))
		parse_canera_object(value, place_to_save);
	else if (!ft_strcmp(name, "figure"))
		parse_figure_object(value, place_to_save);
	else if (!ft_strcmp(name, "light"))
		parse_light_object(value, place_to_save);
	else if (!ft_strcmp(name, "point"))
		parse_point_object(value, place_to_save);
	else if (!ft_strcmp(name, "string"))
		parse_string(value, place_to_save);
	else if (!ft_strcmp(name, "float"))
		parse_float(value, place_to_save);
}

char	*get_object_string(char *string, char *type, void *place_to_save {
	char	*object_end_address;
	int		object_string_len;

	object_end_address = check_brackets(string, 0);
	object_string_len = count_up_to(string, object_end_address);
	parse_object(ft_strsub(string, 0, object_sring_len), type, place_to_save);
	return (object_end_address);
}

void	parse(char *string, t_env *env) {
	ft_strtrim(string);
	if (string[0] != '{')
		ft_err_handler("Scene broken!", 0, 0, 1);
	get_object_string(string, "scene", env);
}
