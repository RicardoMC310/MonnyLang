CPP = g++
FLAGS = -Wall -O0 -g

SRC_DIR = src
INCLUDE_DIR = include
TEMP_DIR = temp
BUILD_DIR = build

SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.cpp')

OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(TEMP_DIR)/%.o, $(SRC_FILES))

TARGET = $(BUILD_DIR)/app

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CPP) $(FLAGS) -I$(INCLUDE_DIR) $^ -o $@

$(TEMP_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(TEMP_DIR) $(BUILD_DIR)

.PHONY: all clean
