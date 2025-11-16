# Compilador
CC=g++
FLAGS_COMPILER=-Wall -Wextra -g -O0
BUILD_SRC=./build
SRC=./src
OBJS_SRC=./objs
INCLUDE_SRC=./include

# Arquivos fonte e objetos - CORRIGIDO
SRC_C_ARQS=$(SRC)/main.cpp

SRC_C_OUT=$(SRC_C_ARQS:$(SRC)/%.cpp=$(OBJS_SRC)/%.o)

BUILD_OUT=$(BUILD_SRC)/monny

# Alvo principal
all: $(BUILD_OUT)

# Link do executável
$(BUILD_OUT): $(SRC_C_OUT) | $(BUILD_SRC)
	$(CC) $(SRC_C_OUT) -o $@ $(FLAGS_COMPILER) \
	-I$(INCLUDE_SRC)

# Compilação dos objetos - CORRIGIDO: pattern rule
$(OBJS_SRC)/%.o: $(SRC)/%.cpp | $(OBJS_SRC)
	$(CC) -c $< -o $@ $(FLAGS_COMPILER) -I$(INCLUDE_SRC)

# Cria diretórios se não existirem
$(BUILD_SRC):
	mkdir -p $(BUILD_SRC)

$(OBJS_SRC):
	mkdir -p $(OBJS_SRC)

# Limpeza
clean:
	rm -rf $(OBJS_SRC)/*.o $(BUILD_OUT)

# Limpeza completa
distclean: clean
	rm -rf $(BUILD_SRC) $(OBJS_SRC)

.PHONY: all clean distclean