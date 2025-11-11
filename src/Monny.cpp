#include <types/throws/SystemError.hpp>
#include <utils/Debug.hpp>
#include <utils/System.hpp>
#include <tokenizer/Scanner.hpp>
#include <Monny.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

void Monny::runScriptFile(const std::string &path)
{
    if (path.empty())
    {
        throw SystemError("File path is empty.");
    }

    if (!fs::exists(path))
    {
        throw SystemError("File not found: " + path);
    }

    if (!fs::is_regular_file(path))
    {
        throw SystemError("Path is not a regular file: " + path);
    }

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        throw SystemError("Cannot open file for reading: " + path);
    }

    std::streamsize size = file.tellg();
    if (size == 0)
    {
        run("");
        return;
    }

    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size))
    {
        throw SystemError("Failed to read file content: " + path);
    }

    file.close();

    std::string source(buffer.begin(), buffer.end());
    run(source);
}

void Monny::runREPL()
{
    std::string line = "";
    std::cout << "monny> ";
    for (;;)
    {
        if (!std::getline(std::cin, line) || line == "exit")
        {
            break;
        }
        if (line == "clear")
        {
            System::clear();
            std::cout << "monny> ";
            continue;
        }
        run(line);
        line = "";
        std::cout << "\nmonny> ";
    }
}

void Monny::run(const std::string &source)
{
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();

    for (auto& token : tokens) 
    {
        Debug::info("[TOKEN]: ", token.toString());
    }
}