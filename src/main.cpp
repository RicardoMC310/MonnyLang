#include <monny.h>
#include <utils/debug.h>
#include <utils/system.h>

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        Debug::error("Usage:", argv[0], "main.mn");
        return 1;
    }

    if (argc == 2)
    {
        Monny::runScriptFile(argv[1]);
    }
    else
    {
        Monny::runREPL();
    }
    return 0;
}