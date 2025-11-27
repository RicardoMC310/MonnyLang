CPP := $(shell command -v ccache >/dev/null 2>&1 && echo "ccache g++" || echo "g++")

INCLUDE_DIR = ./include
SOURCE_DIR  = ./src
TEMP_DIR    = ./temp
BUILD_DIR   = ./build
EXEC_NAME   = monny

COMMON_FLAGS = -Wall -std=c++23 -I$(INCLUDE_DIR)
DEBUG_FLAGS   = $(COMMON_FLAGS) -O0 -g -DDEBUG
RELEASE_FLAGS = $(COMMON_FLAGS) -O3 -DNDEBUG

# Detecta modo
MODE ?= debug

ifeq ($(MODE),debug)
  FLAGS = $(DEBUG_FLAGS)
  TEMP  = $(TEMP_DIR)/debug
  OUT_DIR = $(BUILD_DIR)/debug
else ifeq ($(MODE),release)
  FLAGS = $(RELEASE_FLAGS)
  TEMP  = $(TEMP_DIR)/release
  OUT_DIR = $(BUILD_DIR)/release
endif

# Fontes e objetos
SOURCES := $(shell find $(SOURCE_DIR) -name "*.cpp")
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp,$(TEMP)/%.o,$(SOURCES))
DEPS    := $(OBJECTS:.o=.d)

all: debug

debug:
	@$(MAKE) MODE=debug build

release:
	@$(MAKE) MODE=release build

build: $(OUT_DIR)/$(EXEC_NAME)

$(OUT_DIR)/$(EXEC_NAME): $(OBJECTS)
	@mkdir -p $(OUT_DIR)
	$(CPP) $(FLAGS) -o $@ $^

$(TEMP)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(FLAGS) -MMD -c $< -o $@

-include $(DEPS)

run: debug
	@./$(BUILD_DIR)/debug/$(EXEC_NAME)

clean:
	rm -rf $(TEMP_DIR) $(BUILD_DIR)

.PHONY: all debug release clean run build
