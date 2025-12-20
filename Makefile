CC = gcc
CFLAGS = -Wall -Wextra -I./include -MMD -MP -O0 -Waddress
LDFLAGS = -fsanitize=address

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=objs/%.o)
DEP = $(OBJ:.o=.d)

TARGET_EXE = build/monny

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET_EXE) $(LDFLAGS)

objs/%.o: src/%.c | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs:
	mkdir -p objs
build:
	mkdir -p build

-include $(DEP)

clean:
	rm -rf objs/*.o objs/*.d $(TARGET_EXE)
