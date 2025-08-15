# **************************************************************************** #
#                                  Project Name                                #
# **************************************************************************** #

NAME		= philo
BONUS		= philo_bonus

# **************************************************************************** #
#                             Compiler & Commands                              #
# **************************************************************************** #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -rf
MAKE		= make --no-print-directory
VAL_RUN		= ./philo 5 800 200 200 7
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

VPATH		= src src/utils

INCLUDE_DIR	= includes/
BUILD_DIR	= .build/
VAL_OUT		= log

# **************************************************************************** #
#                                    Files                                     #
# **************************************************************************** #

FILES		+= main
FILES		+= dead
FILES		+= run
FILES		+= loop

UTILS		+= init
UTILS		+= utils

SRC				= $(addsuffix .c, $(FILES)) $(addsuffix .c, $(UTILS))
OBJS			= $(SRC:%.c=$(BUILD_DIR)%.o)

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
	@$(RM) "$(VAL_OUT)";

re: fclean
	@if [ -f $(BONUS) ]; then \
		printf "$(ESC)$(B)$(C_YELLOW)✔ Rebuilding Bonus$(ESC)$(RST)\n"; \
		$(MAKE) bonus; \
	fi
	@printf "$(ESC)$(B)$(C_YELLOW)✔ Rebuilding regular build$(ESC)$(RST)\n"
	@$(MAKE) all

valgrind:
	@if [ ! -f "$(NAME)" ]; then \
		$(MAKE) all; \
	fi
	@if [ -f "$(VAL_OUT)" ]; then \
		rm -f $(VAL_OUT); \
	fi
	@echo "$(ESC)$(B)$(C_YELLOW)Running Valgrind tools with input $(VAL_RUN)$(ESC)$(RST)"
	@echo	"=============================================" >> $(VAL_OUT)
	@echo	" Running Basic Valgrind:" >> $(VAL_OUT)
	@echo	"=============================================" >> $(VAL_OUT)
	@valgrind --leak-check=full --show-leak-kinds=all $(VAL_RUN) 1>> /dev/null 2>> $(VAL_OUT)
	@echo	"$(ESC)$(B)$(C_YELLOW)Finished basic Valgrind$(ESC)$(RST)"
	@echo	"=============================================" >> $(VAL_OUT)
	@echo	" Running Helgrind Tool of Valgrind:" >> $(VAL_OUT)
	@echo	"=============================================" >> $(VAL_OUT)
	@valgrind --tool=helgrind $(VAL_RUN) 1>> /dev/null 2>> $(VAL_OUT)
	@echo "$(ESC)$(B)$(C_YELLOW)Finished Helgrind$(ESC)$(RST)"
	@echo	"=============================================" >> $(VAL_OUT)
	@echo	" Running DRD Tool of Valgrind:" >> $(VAL_OUT)
	@echo	"=============================================" >> $(VAL_OUT)
	@valgrind --tool=drd $(VAL_RUN) 1>> /dev/null 2>> $(VAL_OUT)
	@echo	"$(ESC)$(B)$(C_YELLOW)Finished DRD$(ESC)$(RST)"
	@echo "${BLUE}Valgrind ran on project, output saved to $(VAL_OUT)$(ESC)$(RST)"
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
#	@$(CC) $(CFLAGS) -o $@
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