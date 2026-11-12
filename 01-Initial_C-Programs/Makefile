CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -O2 -g
SRC_DIR := src
BUILD_DIR := build
PROGRAMS := dynamic_array linked_list stack queue my_memcpy my_memmove my_memset my_strcmp my_strcpy my_strlen
TARGETS := $(PROGRAMS:%=$(BUILD_DIR)/%)

.PHONY: all clean
all: $(TARGETS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)
