#include "monny/monny.h"
#include "monny/values.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct monny_state_t
{
    monny_variable_t *memory;
    int memory_capacity, memory_quantity;

    char *buffer_chars;
};

void monny_memory_resize(monny_state_t *state)
{
    size_t old_capacity = state->memory_capacity;
    size_t new_capacity = state->memory_capacity == 0 ? 5 : state->memory_capacity * 2;

    monny_variable_t *new_ptr = (monny_variable_t *)realloc(state->memory, new_capacity * sizeof(monny_variable_t));
    if (!new_ptr)
    {
        perror("monny memory");
        return;
    }

    state->memory = new_ptr;

    memset(
        state->memory + old_capacity,
        0,
        (new_capacity - old_capacity) * sizeof(monny_variable_t));

    state->memory_capacity = new_capacity;
}

monny_state_t *monny_create_state()
{
    monny_state_t *state = (monny_state_t *)calloc(1, sizeof(monny_state_t));
    if (!state)
    {
        perror("monny state");
        return NULL;
    }

    state->memory_capacity = 5;
    state->memory_quantity = 0;
    state->memory = (monny_variable_t *)calloc(state->memory_capacity, sizeof(monny_variable_t));
    if (!state->memory)
    {
        perror("monny memory");
        free(state);
        return NULL;
    }

    return state;
}

void monny_free_state(monny_state_t *state)
{
    if (!state)
        return;

    if (state->memory)
    {
        int i;
        for (i = 0; i < state->memory_quantity; i++)
        {
            monny_variable_t *variable = &state->memory[i];
            if (variable->name)
                free(variable->name);

            if (MONNY_IS_NIL(variable->tagged_value) && MONNY_AS_NIL(variable->tagged_value))
            {
                free(variable->tagged_value.as.ptr);
            }
        }

        free(state->memory);
    }

    if (state->buffer_chars)
        free(state->buffer_chars);

    free(state);
    state = NULL;
}

int monny_save_buffer_file(monny_state_t *state, char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        perror("file");
        return MONNY_ERROR;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    state->buffer_chars = (char *)calloc(file_size, sizeof(char));
    if (!state->buffer_chars)
    {
        perror("monny buffer_chars");
        fclose(file);
        return MONNY_ERROR;
    }

    size_t bytes_readed = fread(state->buffer_chars, sizeof(char), file_size, file);
    fclose(file);

    if (bytes_readed != file_size)
    {
        perror("read file");
        return MONNY_ERROR;
    }

    return MONNY_OK;
}

int monny_load_file(monny_state_t *state, char *path)
{
    if (state == NULL)
        return MONNY_ERROR;

    if (monny_save_buffer_file(state, path) == MONNY_ERROR)
        return MONNY_ERROR;

    

    return MONNY_OK;
}