#include <utils/System.hpp>

void System::clear()
{
#ifdef WIN32
    system("cls");
#else
    system("clear");
#endif
}