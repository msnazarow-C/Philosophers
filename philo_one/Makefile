# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/04 20:40:17 by sgertrud          #+#    #+#              #
#    Updated: 2021/02/01 19:51:45 by sgertrud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re main bonus
export CC CFLAGS MAKEFLAGS LDFLAGS INCLUDES BONUS WITH_BONUS make
make 		= make
CC 			= clang -std=gnu11 -O2
CFLAGS 		= -c -MMD -Wall -Wextra -Werror
BFLAGS		= '-D BONUS=1'
LDFLAGS 	= -MMD -Wall -Wextra -Werror -pthread
DFLAGS 		= '-O0 -g3'
ASFLAGS 	= -fsanitize=address
ifeq ($(CC), gcc)
HFLAGS		= '-pedantic -Wshadow -Wformat=2 -Wfloat-equal\
	-Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align\
	-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2\
	-fsanitize=undefined -fno-sanitize-recover -fstack-protector\
	-Wno-pointer-arith -Wno-cast-qual -Wno-unused-result'
else
HFLAGS		= '-pedantic -Wshadow -Wformat=2 -Wfloat-equal\
	-Wshift-overflow -Wcast-qual -Wcast-align\
	-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2\
	-fsanitize=undefined -fno-sanitize-recover=all -fstack-protector\
	-Wno-pointer-arith -Wno-cast-qual -Wno-unused-result'
endif
MAKEFLAGS	= --no-print-directory
OBJ 		:= main.o max.o ft_atoi.o init_parse.o utils.o
OBJ 		:= $(addprefix obj/,$(OBJ))
NAME 		= philo_one
INCLUDES 	= -I$(PWD)
UNAME 		= $(shell uname)

ifeq ($(UNAME), Linux)
	CFLAGS += -D LINUX=1
endif

all: obj $(NAME)

obj:
	mkdir -p obj

bonus:
	make make='make bonus' CFLAGS+=$(BFLAGS) LDFLAGS+=$(BFLAGS) all

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(INCLUDES) -lm -o $(NAME)

obj/%.o : %.c
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

vanil:
	make CC="gcc " LDFLAGS="-MMD -pthread" CFLAGS="-c -MMD"

debug :
	make bonus CFLAGS+=$(DFLAGS) LDFLAGS+=$(DFLAGS)
debugas :
	make debug CFLAGS+=$(ASFLAGS) LDFLAGS+=$(ASFLAGS)
debugh :
	make debugas CFLAGS+=$(HFLAGS) LDFLAGS+=$(HFLAGS)
norm:
	make fclean && norminette .
clean:
	rm -f *.o *.d
	rm -rf obj

fclean:
	rm -f *.o *.d
	rm -rf obj
	rm -f $(NAME)
re%:
	make fclean
	make $(patsubst re%, %, $@)
re:
	make fclean
	make
