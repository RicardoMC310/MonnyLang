# Compilador
CC=gcc
FLAGS_COMPILER=-Wall -Wextra -g
BUILD_SRC=./build
SRC=./src
OBJS_SRC=./objs
INCLUDE_SRC=./include

# Arquivos fonte e objetos - CORRIGIDO
SRC_C_ARQS=$(SRC)/main.c $(SRC)/monny.c
SRC_C_OUT=$(OBJS_SRC)/main.o $(OBJS_SRC)/monny.o

BUILD_OUT=$(BUILD_SRC)/monny

# Alvo principal
all: $(BUILD_OUT)

# Link do executável
$(BUILD_OUT): $(OBJS_SRC)/main.o $(OBJS_SRC)/monny.o | $(BUILD_SRC)
	$(CC) $(OBJS_SRC)/main.o $(OBJS_SRC)/monny.o -o $@ $(FLAGS_COMPILER) \
	-I$(INCLUDE_SRC)

# Compilação dos objetos - CORRIGIDO: pattern rule
$(OBJS_SRC)/%.o: $(SRC)/%.c | $(OBJS_SRC)
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