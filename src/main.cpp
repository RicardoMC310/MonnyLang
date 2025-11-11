#include <Monny.hpp>
#include <utils/Debug.hpp>
#include <types/throws/SystemError.hpp>
#include <types/throws/SyntaxError.hpp>

int main(int argc, char **argv)
{
    try
    {
        if (argc > 2)
        {
            Debug::error("Usage: ", argv[0], " file.mn");
            std::exit(EXIT_FAILURE);
        }

        if (argc == 2)
        {
            Monny::runScriptFile(argv[1]);
        }
        else
        {
            Monny::runREPL();
        }
    }
    catch (const SystemError &e) 
    {
        Debug::error("[SYSTEM ERROR]: ", e.what());
    }
    catch (const SyntaxError &e) 
    {
        Debug::error("[SYNTAX ERROR]: ", e.what());
    }
    catch (const std::exception &e)
    {
        Debug::error(e.what());
    }

    return EXIT_SUCCESS;
}
