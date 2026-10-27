# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/26 19:04:31 by andrgarc          #+#    #+#              #
#    Updated: 2026/10/26 19:05:23 by andrgarc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -pthread
CPPFLAGS = -I.

SRCS = coder/coder_routine.c \
	coder/coders.c \
	coder/coder_state.c \
	dongle/dongle_queue.c \
	dongle/dongle_request.c \
	dongle/dongles.c \
	dongle/heap.c \
	simulation/monitor.c \
	simulation/simulation_setup.c \
	simulation/simulation_state.c \
	src/main.c \
	src/parse.c \
	utils/arrays.c \
	utils/threads.c \
	utils/times.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.coder
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re
