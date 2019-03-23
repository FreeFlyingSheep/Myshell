NAME = myshell
INC_DIR = ./include
BUILD_DIR = ./build

OBJ_DIR = $(BUILD_DIR)/obj
BINARY = $(BUILD_DIR)/$(NAME)

.DEFAULT_GOAL = $(NAME)

# Compilation flags
CC = gcc
LD = gcc
INCLUDES = $(addprefix -I, $(INC_DIR))
CFLAGS   = -O2 -MMD -Wall -ggdb3 $(INCLUDES)

# Files to be compiled
SRCS = $(shell find src/ -name "*.c")
OBJS = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# Compilation patterns
$(OBJ_DIR)/%.o: src/%.c
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

# Depencies
-include $(OBJS:.o=.d)

.PHONY: $(NAME) clean

$(NAME): $(BINARY)

$(BINARY): $(OBJS)
	@echo + LD $@
	@$(LD) -O2 -o $@ $^

clean:
	-rm -rf $(BUILD_DIR)