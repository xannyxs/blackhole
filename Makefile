NAME = blackhole_sim
CC = gcc

CFLAGS = -Wall -Wextra -Werror -g -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lglfw -lGL -lm -ldl -lpthread -lX11

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJ_DIR = build

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking executable..."
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "Done! Run with ./$(NAME)"

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

