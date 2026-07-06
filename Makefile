NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -I includes
LFLAGS = -pthread
RM = rm -f
SRCDIR = src

SRCS = main.c init.c actions.c dining.c monitor.c watcher.c utils.c cleanup.c
OBJS = $(addprefix $(SRCDIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(NAME) $(OBJS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c includes/philosophers.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
