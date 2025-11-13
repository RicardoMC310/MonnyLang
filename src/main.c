#include <monny.h>

int main(int argc, char **argv)
{

    if (argc > 2)
    {
        printf("Usage: %s main.mn\n", argv[0]);
        return 1;
    }

    Monny *monny = createMonny();

    if (argc == 2)
    {
        runScriptFile(monny, argv[1]);
    }
    else
    {
        runREPL(monny);
    }

    destroyMonny(monny);

    return 0;
}