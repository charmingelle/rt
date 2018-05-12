# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/10 17:05:19 by pgritsen          #+#    #+#              #
#    Updated: 2018/05/12 14:19:54 by pgritsen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	RT

CC			=	gcc

OS			=	$(shell uname)

ifeq ($(OS), Linux)
	# CFLAGS		=	-Wextra -Wall -O3 -g3
	CFLAGS		=	-O3 -g3
else
	# CFLAGS		=	-Wextra -Werror -Wall -O3 -g3
	CFLAGS		=	-O3 -g3
endif

HDRSDIR		=	./includes

HDRS		=	$(addprefix $(HDRSDIR)/, rt.h structures.h)

SRCSDIR		=	./sources

SRCS		=	main.c events.c stats.c utils.c draw.c opencl.c sys_handler.c		\
				cleaner.c movement.c vec.c											\
				ann_scene.c
				# scene.c

OBJDIR		=	./obj

OBJ			=	$(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

LIBFOLDER	=	./libraries

LIBSDEPS	=	$(addprefix $(LIBFOLDER)/, libft/libft.a sgl/libsgl.a)

INCLUDES	=	-I./includes
INCLUDES	+=	$(addprefix -I$(LIBFOLDER)/, libft sgl)

ifeq ($(OS), Linux)
	FRAMEWORKS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm -lOpenCL
else
	INCLUDES	+=	-I./frameworks/SDL2.framework/Headers
	INCLUDES	+=	-I./frameworks/SDL2_ttf.framework/Headers
	INCLUDES	+=	-I./frameworks/SDL2_image.framework/Headers
	INCLUDES	+=	-I./frameworks/SDL2_mixer.framework/Headers
	INCLUDES	+=	-F./frameworks

	FRAMEWDIR	=	frameworks

	FRAMEWORKS	=	$(addprefix -F./, $(FRAMEWDIR))
	FRAMEWORKS	+=	$(addprefix -rpath ./, $(FRAMEWDIR))
	FRAMEWORKS	+=	-framework OpenGL -framework AppKit -framework OpenCl		\
					-framework SDL2 -framework SDL2_ttf -framework SDL2_image -framework SDL2_mixer
endif

LIBRARIES	=	$(addprefix -L$(LIBFOLDER)/, libft sgl) -lft -lsgl

all: lib $(NAME)

$(NAME): $(OBJDIR) $(OBJ) $(HDRS) $(LIBSDEPS)
	@printf "\n\033[32m[Creating $(NAME)].......\033[0m"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBRARIES) $(FRAMEWORKS)
	@printf "\033[32m[DONE]\033[0m\n"

$(OBJDIR):
	@printf "\n\033[32m[Creating folder $(OBJDIR)].......\033[0m"
	@mkdir $(OBJDIR)
	@printf "\033[32m[DONE]\033[0m\n"

$(OBJ): $(OBJDIR)/%.o : $(SRCSDIR)/%.c $(HDRS)
	@printf "\033[32m[Compiling $<].......\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)
	@printf "\033[32m[DONE]\033[0m\n"

$(LIBSDEPS): lib

lib:
	@printf "\033[33m[Creating libft.a].......[START]\033[0m\n"
	@make -j3 -C $(LIBFOLDER)/libft
	@printf "\033[33m[Creating libft.a].......[END]\033[0m\n\n"
	@printf "\033[33m[Creating libsgl.a].......[START]\033[0m\n"
	@make -j3 -C $(LIBFOLDER)/sgl
	@printf "\033[33m[Creating libsgl.a].......[END]\033[0m\n"

clean:
	@printf "\n\033[31m[Cleaning $(NAME) object files].......\033[0m"
	@rm -rf $(OBJDIR)
	@printf "\033[31m[DONE]\033[0m\n"

clean_%:
	@make -j3 -C $(LIBFOLDER)/$(subst clean_,,$@) clean

fclean: clean
	@rm -rf $(NAME)
	@printf "\033[31m[Removing $(NAME)].......[DONE]\033[0m\n"

fclean_%:
	@make -j3 -C $(LIBFOLDER)/$(subst fclean_,,$@) fclean

rebuild_message:
	@printf "\033[33m[Rebuild $(NAME)].......[START]\033[0m\n"

re:	rebuild_message fclean all
	@printf "\033[33m[Rebuild $(NAME)].......[END]\033[0m\n"

re_%:
	@make -j3 -C $(LIBFOLDER)/$(subst re_,,$@) re

.NOTPARALLEL: all $(NAME) re
