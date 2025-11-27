#include <utils/system.h>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

void System::clear()
{
#ifdef _WIN32
    HANDLE hConsole = GetStdhandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD _unused, cellCount;
    COORD homeCoords = {0, 0};

    if (hConsole == INVALID_HANDLE_VALUE)
        return;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;

    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', cellCount, homeCoords, &_unused))
        return;
    
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &_unused))
        return;
    
    SetConsoleCursorPosition(hConsole, homeCoords);
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}