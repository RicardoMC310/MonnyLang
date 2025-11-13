#include <monny.h>

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage: %s main.mn\n", argv[0]);
        return 1;
    }
    else
    {
        MonnyState *M = monny_new();

        if (monny_load_file(M, argv[1]) < 0)
        {
            fprintf(stderr, "Erro: %s\n", monny_get_error(M));
            monny_close(M);
            return 1;
        }
        
        monny_close(M);
    }

    return 0;
}