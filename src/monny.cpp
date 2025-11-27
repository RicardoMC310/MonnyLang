#include <monny.h>
#include <utils/debug.h>
#include <utils/system.h>

#include <filesystem>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <functional>

namespace fs = std::filesystem;

void Monny::runScriptFile(const std::string &path)
{
    if (!fs::exists(path))
    {
        Debug::error("Arquivo", path, "não encontrado");
        exit(66);
    }

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        Debug::error("Arquivo", path, "sem permissão de escrita");
        exit(66);
    }

    std::streamsize size = file.tellg();
    file.seekg(std::ios::beg);

    size = size < 0 ? 0 : size;

    std::vector<char> buffer(size);

    if (!file.read(buffer.data(), size))
    {
        Debug::error("Arquivo", path, "está corrompido");
        exit(77);
    }

    std::string source{buffer.begin(), buffer.end()};
    Monny::run(source);
}

void Monny::runREPL()
{
    std::string line = "";
    std::cout << "monny> ";

    std::unordered_map<std::string, std::function<void()>> allowFunctionsConsole = {
        {"clear", []() {System::clear();}}
    };

    for (;;) 
    {
        if (!std::getline(std::cin, line) || line == "exit")
        {
            break;
        }
        
        auto it = allowFunctionsConsole.find(line);
        if (it != allowFunctionsConsole.end()) {
            it->second();
            line = "";
            std::cout << "\nmonny> ";
            continue;
        }

        Monny::run(line);
        line = "";
        std::cout << "\nmonny> ";
    }
}

void Monny::run(const std::string &source)
{
    Debug::info(source);
}