#ifndef _WIN32

#include "monny/core/process.h"

#include <pty.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>

Process monny_process_spawn(char *argv[])
{

    int master_fd;

    pid_t pid = forkpty(&master_fd, NULL, NULL, NULL);
    if (pid < 0)
    {
        perror("forkpty");
        exit(1);
    }

    if (pid == 0)
    {
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    }

    struct termios t;
    tcgetattr(master_fd, &t);
    t.c_lflag &= ~ECHO;
    tcsetattr(master_fd, TCSANOW, &t);

    Process p = {
        .pty_master = master_fd,
        .pid = pid};

    return p;
}

void monny_process_write(Process *process, const char *text)
{
    write(process->pty_master, text, strlen(text));
}

char *monny_process_read(Process *process)
{
    static char buffer[2048];

    ssize_t n = read(process->pty_master, buffer, sizeof(buffer) - 1);
    if (n <= 0)
        return NULL;

    buffer[n] = '\0';
    return buffer;
}

void monny_process_close_stdin(Process *process)
{
    close(process->pty_master);
}

void monny_process_end(Process *process)
{
    close(process->pty_master);
    int status;
    waitpid(process->pid, &status, 0);
}

#elifdef _WIN32

#include "monny/core/process.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Process monny_process_spawn(char *argv[])
{
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    HANDLE child_stdin_read, child_stdin_write;
    HANDLE child_stdout_read, child_stdout_write;

    // Cria pipes
    if (!CreatePipe(&child_stdin_read, &child_stdin_write, &sa, 0))
    {
        fprintf(stderr, "CreatePipe stdin failed.\n");
        exit(1);
    }
    if (!CreatePipe(&child_stdout_read, &child_stdout_write, &sa, 0))
    {
        fprintf(stderr, "CreatePipe stdout failed.\n");
        exit(1);
    }

    // Não herdar as pontas usadas pelo pai
    SetHandleInformation(child_stdin_write, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(child_stdout_read, HANDLE_FLAG_INHERIT, 0);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = child_stdin_read;
    si.hStdOutput = child_stdout_write;
    si.hStdError = child_stdout_write;

    // Constrói command line
    char cmdline[1024] = "";
    for (int i = 0; argv[i]; i++)
    {
        if (i > 0)
            strcat(cmdline, " ");
        strcat(cmdline, argv[i]);
    }

    if (!CreateProcessA(NULL, cmdline, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        fprintf(stderr, "CreateProcess failed: %lu\n", GetLastError());
        exit(1);
    }

    // Fecha as pontas usadas pelo filho
    CloseHandle(child_stdin_read);
    CloseHandle(child_stdout_write);

    Process p = {
        .in_write = child_stdin_write,
        .out_read = child_stdout_read,
        .process_handle = pi.hProcess,
        .thread_handle = pi.hThread};

    return p;
}

void monny_process_write(Process *p, const char *text)
{
    DWORD written;
    WriteFile(p->in_write, text, (DWORD)strlen(text), &written, NULL);
}

char *monny_process_read(Process *p)
{
    static char buffer[2048];
    DWORD readBytes;

    if (!ReadFile(p->out_read, buffer, sizeof(buffer) - 1, &readBytes, NULL) || readBytes == 0)
        return NULL;

    buffer[readBytes] = '\0';
    return buffer;
}

void monny_process_close_stdin(Process *p)
{
    if (p->in_write != INVALID_HANDLE_VALUE)
        CloseHandle(p->in_write);
    p->in_write = INVALID_HANDLE_VALUE;
}

void monny_process_end(Process *p)
{
    if (p->in_write != INVALID_HANDLE_VALUE)
        CloseHandle(p->in_write);
    if (p->out_read != INVALID_HANDLE_VALUE)
        CloseHandle(p->out_read);

    WaitForSingleObject(p->process_handle, INFINITE);
    CloseHandle(p->process_handle);
    CloseHandle(p->thread_handle);
}

#endif
