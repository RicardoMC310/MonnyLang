#include <Monny.h>

int main(int argc, char **argv)
{

    if (argc > 2)
    {
        std::cout << "[ERROR]: usage " << argv[0] << " script.mn\n";
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