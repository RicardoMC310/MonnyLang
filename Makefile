# ============================
#          Cores
# ============================

BOLD=\033[1m
GREEN=\033[1;32m
YELLOW=\033[1;33m
BLUE=\033[1;34m
RED=\033[1;31m
PURPLE=\033[1;35m
RESET=\033[0m

# ============================
#        Config Geral
# ============================

CC=gcc
CXX=g++
SRC=./src
OBJS=./objs
BUILD=./build
INCLUDE_SRC=./include

# Detectar fontes
SRC_C=$(shell find $(SRC) -type f -name '*.c')
SRC_CPP=$(shell find $(SRC) -type f -name '*.cpp')

OBJ_C=$(SRC_C:$(SRC)/%.c=$(OBJS)/%.o)
OBJ_CPP=$(SRC_CPP:$(SRC)/%.cpp=$(OBJS)/%.o)
OBJS_ALL=$(OBJ_C) $(OBJ_CPP)

OUT=$(BUILD)/monny

# ============================
#     Flags Debug / Release
# ============================

MODE ?= debug

ifeq ($(MODE),debug)
    MODE_COLOR=$(YELLOW)
    CFLAGS=-Wall -Wextra -g -O0 -I$(INCLUDE_SRC) #-DMONNY_DEBUG_TOKENS
    CPPFLAGS=$(CFLAGS)
    MODE_NAME="DEBUG"
else ifeq ($(MODE),release)
    MODE_COLOR=$(GREEN)
    CFLAGS=-Wall -Wextra -O3 -DNDEBUG -I$(INCLUDE_SRC)
    CPPFLAGS=$(CFLAGS)
    MODE_NAME="RELEASE"
else
    $(error MODE inválido! Use 'debug' ou 'release')
endif

# ============================
#          Alvos
# ============================

all: debug

debug:
	@$(MAKE) MODE=debug build_target

release:
	@$(MAKE) MODE=release build_target

# ⚠️ Renomeado para evitar colisão com diretório "build"
build_target: $(OUT)

# Linkagem
$(OUT): $(OBJS_ALL) | $(BUILD)
	@echo -e "$(MODE_COLOR)[LINK $(MODE_NAME)]$(RESET) $@"
	@$(CC) $(OBJS_ALL) -o $@

# Compilar .c
$(OBJS)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	@echo -e "$(MODE_COLOR)[CC $(MODE_NAME)]$(RESET)  $<"
	@$(CC) -c $< -o $@ $(CFLAGS) -MMD -MP

# Compilar .cpp
$(OBJS)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	@echo -e "$(MODE_COLOR)[CXX $(MODE_NAME)]$(RESET) $<"
	@$(CXX) -c $< -o $@ $(CPPFLAGS) -MMD -MP

# Incluir arquivos .d
-include $(OBJS_ALL:.o=.d)

# Diretório de build
$(BUILD):
	@mkdir -p $(BUILD)

# ============================
#          Limpeza
# ============================

clean:
	@echo -e "$(RED)[CLEAN]$(RESET) Objetos removidos."
	@rm -rf $(OBJS)

distclean: clean
	@echo -e "$(RED)[CLEAN]$(RESET) Build removido."
	@rm -rf $(BUILD)

.PHONY: all clean distclean debug release build_target
