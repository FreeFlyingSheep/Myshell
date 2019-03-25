ifeq ($(NAME), )
$(error Should make in echo app's directory)
endif

BUILD_DIR = ./build

OBJ_DIR = $(BUILD_DIR)/obj
BINARY = $(BUILD_DIR)/$(NAME)

CC = gcc
LD = gcc
CFLAGS = -Wall -ggdb3

SRCS = $(shell find ./ -name "*.c")
INCS = $(shell find ./ -name "*.h")
OBJS = $(SRCS:./%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: ./%.c
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: $(NAME) clean

$(NAME): $(BINARY)

$(BINARY): $(OBJS) $(INCS)
	@echo + LD $@
	@$(LD) -o $@ $^

clean:
	-rm -rf $(BUILD_DIR)