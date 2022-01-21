# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/05 19:44:51 by kdustin           #+#    #+#              #
#    Updated: 2022/01/20 17:12:14 by kdustin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ######################### Settings ######################################### #

NAME = scop

CC = g++

PROJECT_SOURCES = ./sources
PROJECT_INCLUDES = ./includes
PROJECT_OBJECTS = ./objects

CPP = -std=c++2a
CFLAGS = -Wall -Wextra -Werror ${CPP}

GLFW_INCLUDE = ./glfw3/include
GLFW_LIB = ./glfw3/build/src
LIB_FLAGS = -L$(GLFW_LIB) -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

CFLAGS += $(LIB_FLAGS)

FT_CONTAINERS = ./ft_containers

RT_MATH = ./rt_math

PROJECT_INCLUDES += $(GLFW_INCLUDE) $(FT_CONTAINERS) $(RT_MATH)

DEBUG_FLAGS =
VALGRIND_ARGS = "config.scop"

CEXTENSION = cpp
HEXTENSION = hpp

GITIGNORE_DATA = $(NAME)\n.norm_script.sh\n*.o\n*.d\nobjs\ndpds\n.idea\n*.dSYM\n*.test*\ncmake-build-debug\n*.a\n.DS_Store

# ############################################################################ #

SRCS = $(shell find $(PROJECT_SOURCES) -name '*.$(CEXTENSION)')
OBJS = $(patsubst $(PROJECT_SOURCES)/%,$(PROJECT_OBJECTS)/%, $(SRCS:%.$(CEXTENSION)=%.o))
HDRS = $(wildcard $(PROJECT_INCLUDES)/*.$(HEXTENSION))

INCS = $(PROJECT_INCLUDES)
INCLUDES = $(foreach i,$(INCS),-I$(i))

all: $(NAME)

$(NAME): $(PROJECT_OBJECTS) $(OBJS) $(HDRS)
	@echo "${SUCCESS}Compiling done!${BREAK_COLOR}"

	@echo "${INFO}Linking...${BREAK_COLOR}"
	$(CC) $(OBJS) $(CFLAGS) $(INCLUDES) -o $(NAME)
	@echo "${SUCCESS}Linking done!${BREAK_COLOR}"

IS_COMPILING_START := 0
$(PROJECT_OBJECTS)/%.o: $(PROJECT_SOURCES)/%.$(CEXTENSION)
	@if [ "$(IS_COMPILING_START)" = "0" ]; then\
		echo "${INFO}Compiling...${BREAK_COLOR}";\
	fi
	$(eval IS_COMPILING_START := 1)
	@echo "${INFO}Compile $@...${BREAK_COLOR}"
	$(CC) ${CPP} $(INCLUDES) $(DEBUG_FLAGS) -c $(PROJECT_SOURCES)/$*.$(CEXTENSION) -o $(PROJECT_OBJECTS)/$*.o

$(PROJECT_OBJECTS):
	@echo "${INFO}Create objects directory...${BREAK_COLOR}"
	@mkdir -p $(PROJECT_OBJECTS)

clean:
	@echo "${WARNING}Deleting objects...${BREAK_COLOR}"
	@rm -rf $(PROJECT_OBJECTS)

fclean: clean
	@echo "${WARNING}Deleting  $(NAME)...${BREAK_COLOR}"
	@rm -f $(NAME)

re: fclean all

########################### Debug ######################################################################################
debug: DEBUG_FLAGS += -g
debug: re
valgrind: debug
	 @valgrind --leak-check=full --show-leak-kinds=all \
	 --dsymutil=yes --track-origins=yes ./$(NAME) $(VALGRIND_ARGS)
test:
	#cmake -S . -B build
	cd tests && cmake --build build && cd build && ctest
########################### Color Scheme ###############################################################################

DANGER = \033[0;31m
SUCCESS = \033[0;32m
WARNING = \033[0;33m
INFO = \033[0;34m
BREAK_COLOR = \033[0m

.PHONY: all clean fclean re debug sanitize valgrind
