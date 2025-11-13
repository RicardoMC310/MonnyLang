#include <monny.h>

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class MonnyImpl
{
private:
    void run(const std::string &source)
    {
        std::cout << source << std::endl;
    }

public:
    void runScriptFile(const char *&path)
    {
        if (!fs::exists(path))
        {
            std::cout << "Arquivo não foi encontrado!\n";
            exit(1);
        }

        if (!fs::is_regular_file(path))
        {
            std::cout << "Caminho passado não é um arquivo válido!\n";
            exit(1);
        }

        fs::path filePath(path);
        if (filePath.extension().string() != ".mn")
        {
            std::cout << "Formato inválido, aceito apenas '.mn'\n";
            exit(1);
        }

        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            std::cout << "Arquivo não tem permissão de leitura.\n";
            exit(1);
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);

        if (!file.read(buffer.data(), size))
        {
            std::cout << "Erro ao ler arquivo, pode estar corrompido.\n";
            exit(1);
        }

        std::string souce{buffer.begin(), buffer.end()};
        run(souce);
    }

    void runREPL()
    {
        std::string line;
        std::cout << "monny> ";

        for(;;)
        {
            if (!std::getline(std::cin, line) || line == "exit")
            {
                break;
            }
            
            run(line);
            line = "";
            std::cout << "\nmonny> ";
        }
    }
};

extern "C"
{
    Monny *createMonny()
    {
        return reinterpret_cast<Monny *>(new MonnyImpl());
    }

    void destroyMonny(Monny *monny)
    {
        auto impl = reinterpret_cast<MonnyImpl *>(monny);
        delete impl;
    }

    void runScriptFile(Monny *monny, const char *path)
    {
        auto impl = reinterpret_cast<MonnyImpl *>(monny);
        impl->runScriptFile(path);
    }

    void runREPL(Monny *monny)
    {
        auto impl = reinterpret_cast<MonnyImpl *>(monny);
        impl->runREPL();
    }
}