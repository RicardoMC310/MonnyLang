#include <monny.h>
#include <scanner.h>
#include <parser.h>
#include <bytecode.h>
#include <vm.h>

struct MonnyState
{
    int is_running;
    char *last_error;
};

MonnyState *monny_new(void)
{
    MonnyState *M = malloc(sizeof(MonnyState));
    if (M == NULL)
        return NULL;

    M->is_running = true;
    M->last_error = NULL;

    return M;
}

void monny_close(MonnyState *M)
{
    if (M->last_error)
        free(M->last_error);

    free(M);
    M = NULL;
}

static int load_file_to_buffer(const char *filename, char **buffer, size_t *size)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
        return -1;

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *buffer = malloc(*size + 1);
    if (!*buffer)
    {
        fclose(file);
        return -1;
    }

    fread(*buffer, 1, *size, file);
    (*buffer)[*size] = '\0';
    fclose(file);
    return 0;
}

int monny_load_file(MonnyState *M, const char *filename)
{
    char *source;
    size_t size;

    if (load_file_to_buffer(filename, &source, &size) < 0)
    {
        if (M->last_error) free(M->last_error);
        M->last_error = strdup("Cannot open file");
        return -1;
    }

    Scanner *scanner = createScanner(source, size);
    if (scanTokens(scanner) < 0)
    {
        if (M->last_error) free(M->last_error);
        M->last_error = getLastErrorScanner(scanner);
        return 1;
    }

    Token *tokens = getTokens(scanner);
    size_t tokensCount = getTokensCount(scanner);

    Parser *parser = createParser(tokens, tokensCount);
    ASTNode *node = parse(parser);

    BytecodeChunck *chunck = generateCode(node);

    VM *vm = createVM();

    execute(vm, chunck);

    free(source);
    freeAST(node);
    destroyVM(vm);
    destroyChunck(chunck);
    destroyParser(parser);
    destroyScanner(scanner);

    return 0;
}

char* monny_get_error(MonnyState *M)
{
    if (M->last_error) return M->last_error;
    return "\0";
}