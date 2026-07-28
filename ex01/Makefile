NAME		= easyfind

#********************************************************************************#

CC			= c++
CFLAGS		= -Wall -Wextra -Werror -fsanitize=address -std=c++98
DEPFLAGS	= -MMD -MP -MF $(DDIR)/$*.d

#********************************************************************************#

SDIR		= src
ODIR		= obj
DDIR		= deps

#***********************************************x*********************************#

SRC			= src/main.cpp
HEADER		= src/BitcoinExchange.hpp

OBJ			= $(SRC:%.cpp=$(ODIR)/%.o)
DEPS		= $(OBJ:%.o=$(DDIR)/%.d)

#********************************************************************************#

all : $(NAME)

$(NAME) : Makefile $(OBJ)| $(ODIR) $(DDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo "\033[32m ✌ CPP09 EX00 COMPILED!\033[0m"

$(ODIR) $(DDIR):
	@mkdir -p $@

$(ODIR)/%.o: %.cpp $(HEADER) Makefile | $(ODIR) $(DDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DDIR)/$*.d)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

clean :
	@rm -rf $(ODIR) $(DDIR)
	@echo "\033[1;31m Files cleaned ✕ \033[0m"

fclean : clean
	@rm -rf $(NAME)

re : fclean all

-include $(DEPS)

.PHONY : all clean fclean re