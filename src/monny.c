#include "monny/monny.h"
#include "monny/values.h"
#include "monny/tokenizer/scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct monny_state_t
{
    monny_variable_t *memory;
    size_t memory_capacity;
    size_t memory_quantity;

    char *buffer_chars;
};

monny_state_t *monny_create_state()
{
    monny_state_t *state = calloc(1, sizeof(monny_state_t));
    if (!state)
    {
        perror("create monny state");
        return NULL;
    }

    state->memory_capacity = 8;
    state->memory_quantity = 0;
    state->memory = calloc(state->memory_capacity, sizeof(monny_variable_t));
    if (!state->memory)
    {
        perror("monny memory");
        free(state);
        return NULL;
    }

    state->buffer_chars = NULL;
    return state;
}

void monny_free_state(monny_state_t *state)
{
    if (!state)
        return;

    if (state->memory)
    {
        for (size_t i = 0; i < state->memory_quantity; i++)
        {
            monny_variable_t *var = &state->memory[i];

            if (var->name)
                free(var->name);

            if (MONNY_IS_STRING(var->tagged_value))
            {
                // Agora não precisamos free da string do token
                // Só liberamos strings realmente alocadas para variáveis
                if (var->tagged_value.as.string)
                    free(var->tagged_value.as.string);
            }
        }
        free(state->memory);
    }

    if (state->buffer_chars)
        free(state->buffer_chars);

    free(state);
}

static int monny_save_buffer_file(monny_state_t *state, const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        perror("file open");
        return MONNY_ERROR;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    state->buffer_chars = calloc(file_size + 1, sizeof(char));
    if (!state->buffer_chars)
    {
        perror("buffer alloc");
        fclose(file);
        return MONNY_ERROR;
    }

    size_t readed = fread(state->buffer_chars, 1, file_size, file);
    fclose(file);

    if (readed != file_size)
    {
        perror("file read");
        free(state->buffer_chars);
        state->buffer_chars = NULL;
        return MONNY_ERROR;
    }

    state->buffer_chars[file_size] = '\0';
    return MONNY_OK;
}

int monny_load_file(monny_state_t *state, char *path)
{
    if (!state)
        return MONNY_ERROR;

    if (state->buffer_chars)
    {
        free(state->buffer_chars);
        state->buffer_chars = NULL;
    }

    if (monny_save_buffer_file(state, path) != MONNY_OK)
        return MONNY_ERROR;

    monny_scanner_t *scanner = monny_create_scanner();
    if (!scanner)
        return MONNY_ERROR;

    monny_tokens_t *tokens = monny_scan_tokens(scanner, state->buffer_chars, (int)strlen(state->buffer_chars));
    (void)tokens;

    monny_free_scanner(scanner);
    return MONNY_OK;
}
