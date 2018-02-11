# default compiler
CC := g++

# folders
SOURCE_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE_DIR := include


# srcs, objs, bins
ENCRYPT_DECRYPT_BIN := $(BIN_DIR)/encrypt_decrypt
BRUTEFORCE_BIN := $(BIN_DIR)/bruteforce

# compile flags
CPPFLAGS := -std=c++11 -c -Wall -pthread
INC := -Iinclude

all: $(ENCRYPT_DECRYPT_BIN) $(BRUTEFORCE_BIN)

$(ENCRYPT_DECRYPT_BIN): $(BUILD_DIR)/md5.o $(BUILD_DIR)/encrypt_decrypt.o
	@echo "Linking..."
	@mkdir -p $(BIN_DIR)
	$(CC) $(BUILD_DIR)/md5.o $(BUILD_DIR)/encrypt_decrypt.o -o $@

$(BRUTEFORCE_BIN): $(BUILD_DIR)/bruteforce.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(BUILD_DIR)/bruteforce.o -o $@	

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: clean all
