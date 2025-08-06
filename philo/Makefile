# **************************************************************************** #
#                                  Project Name                                #
# **************************************************************************** #

NAME		= philo
BONUS		= philo_bonus

# **************************************************************************** #
#                             Compiler & Commands                              #
# **************************************************************************** #

CC			= cc
CFLAGS		= -Wall -Wextra
RM			= rm -rf
MAKE		= make --no-print-directory
LOGO_BUILT	= 0
BONUS_BUILT	= 0

# **************************************************************************** #
#                                 Output Formatting                            #
# **************************************************************************** #

ESC			= \033[
RST			= 0m

# Styles
B			= 1;#	Bold
D			= 2;#	Dim
I			= 3;#	Italic
U			= 4;#	Underline
BL			= 5;#	Blink
ST			= 9;#	Strikethrough

# Colors
C_MAGENTA	= 95m#	Cleaned files but program still works
C_RED		= 91m#	Program doesn't work anymore
C_GREEN		= 92m#	Program is working
C_YELLOW	= 93m#	Message
C_LOGO		= 38;5;217m

# **************************************************************************** #
#                                 Directories                                  #
# **************************************************************************** #

VPATH		= src

INCLUDE_DIR	= includes/
BUILD_DIR	= .build/

# **************************************************************************** #
#                                    Files                                     #
# **************************************************************************** #

FILES		+= main
FILES		+= dead
FILES		+= init

SRC				= $(addsuffix .c, $(FILES))
OBJS			= $(addprefix $(BUILD_DIR), $(addsuffix .o, $(FILES)))

INCLUDE	= -I$(INCLUDE_DIR)

# **************************************************************************** #
#                                    Rules                                     #
# **************************************************************************** #

all: $(NAME)
	@if [ "$(LOGO_BUILT)" = "1" ]; then \
		$(MAKE) logo; \
	else \
		echo "$(ESC)$(B)$(C_YELLOW)✔ Everything is up to date$(ESC)$(RST)"; \
	fi

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@
	@$(eval LOGO_BUILT := 1)
	@printf "$(ESC)$(B)$(C_GREEN)✔ Created binary: $(ESC)$(RST)$@\n"

$(BUILD_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@printf "\r$(ESC)$(C_YELLOW)✔ Compiled →$(ESC)$(RST) $< → $@$(ESC)$(RST)\n"

clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		$(RM) "$(BUILD_DIR)"; \
		printf "\n$(ESC)$(B)$(C_MAGENTA)✔ Cleaned build directory$(ESC)$(RST)\n"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ] || [ -f "$(BONUS)" ]; then \
		$(RM) "$(NAME)"; \
		$(RM) "$(BONUS)"; \
		printf "\n$(ESC)$(B)$(C_RED)✔ Removed binaries$(ESC)$(RST)\n\n"; \
	fi

re: fclean
	@if [ -f $(BONUS) ]; then \
		printf "$(ESC)$(B)$(C_YELLOW)✔ Rebuilding Bonus$(ESC)$(RST)\n"; \
		$(MAKE) bonus; \
	fi
	@printf "$(ESC)$(B)$(C_YELLOW)✔ Rebuilding regular build$(ESC)$(RST)\n"
	@$(MAKE) all

# **************************************************************************** #
#                                     Bonus                                    #
# **************************************************************************** #

#bonus: $(BONUS)
#	@if [ "$(LOGO_BUILT)" = "1" ]; then \
		$(MAKE) logo; \
	else \
		echo "$(ESC)$(B)$(C_YELLOW)✔ Everything is up to date$(ESC)$(RST)"; \
	fi

#$(BONUS): $(BONUS_OBJS)
#	@$(CC) $(CFLAGS) $^ $(MLX_FLAGS) -o $@
#	@$(eval LOGO_BUILT := 1)
#	@printf "$(ESC)$(B)$(C_GREEN)✔ Created binary: $(ESC)$(RST)$@\n"

# **************************************************************************** #
#                                     Logo                                     #
# **************************************************************************** #

logo:
	@printf "$(ESC)$(B)$(C_LOGO) \
	    _   _   _   _   _   _   _   _   _   _   _   _  \n \
	   / \ / \ / \ / \ / \ / \ / \ / \ / \ / \ / \ / \ \n \
	  ( P | h | i | l | o | s | o | p | h | e | r | s )\n \
	   \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \n \
	$(ESC)$(RST)"
	@printf "\
	                                 By: Miguel Azuaga\n"

# **************************************************************************** #
#                                   Phony Targets                              #
# **************************************************************************** #

.PHONY: all clean fclean re download logo