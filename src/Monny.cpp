#include <Monny.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

namespace fs = std::filesystem;

void Monny::runScriptFile(const std::string &path)
{
    if (!fs::exists(path))
    {
        // TODO(ricardo): Fazer logger
        std::cout << "[ERROR] File " << path << " not found\n";
        std::exit(77);
    }

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        // TODO(ricardo): Fazer logger
        std::cout << "[ERROR] File " << path << " not a permission for read\n";
        std::exit(77);
    }

    std::streamsize size = file.tellg();
    file.seekg(std::ios::beg);

    size = size <= 0 ? 0 : size;

    std::vector<char> buffer(size);

    if (!file.read(buffer.data(), size))
    {
        // TODO(ricardo): Fazer logger
        std::cout << "[ERROR] File " << path << " as corrupted\n";
        std::exit(66);
    }

    std::string source{buffer.begin(), buffer.end()};
    Monny::run(source);
}

void Monny::runREPL()
{
    std::string line;

    std::cout << "monny> ";
    for (;;)
    {
        if (!std::getline(std::cin, line) || line == "exit")
        {
            break;
        }

        Monny::run(line);
        line = "";
        std::cout << "\nmonny> ";
    }
}

void Monny::run(const std::string &source)
{
    std::cout << source << '\n';
}