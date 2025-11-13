#ifndef _MONNY_H_
#define _MONNY_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct Monny Monny;

    Monny *createMonny();
    void destroyMonny(Monny *monny);

    void runScriptFile(Monny *monny, const char *path);
    void runREPL(Monny *monny);

#ifdef __cplusplus
}
#endif

#endif // _MONNY_H_