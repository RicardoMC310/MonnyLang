#ifndef PROCESS_H
#define PROCESS_H

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#endif

typedef struct Process
{
#ifdef _WIN32
    HANDLE hPipeIn;
    HANDLE hPipeOut;
    HANDLE hProcess;
    HANDLE hThread;
#else
    int pty_master;
    pid_t pid;
#endif
} Process;

Process monny_process_spawn(char *argv[]);
void monny_process_write(Process *process, const char *text);
char *monny_process_read(Process *process);

void monny_process_close_stdin(Process *process);
void monny_process_end(Process *process);

#endif
