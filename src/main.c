#include "monny/core/process.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *reader(void *data)
{
    Process *p = (Process *)data;

    char *buffer;
    while ((buffer = monny_process_read(p)) != NULL)
    {
        printf("%s", buffer);
    }

    return NULL;
}

void *writer(void *data)
{
    Process *p = (Process *)data;

    char *code =
        "print('Hello')\n"
        "def soma(x: int, y: int) -> int:\n"
        "    return x + y\n"
        "print(soma(1, 2))\n"
        "print(soma(1, 7))\n"
        "exit()\n";

    monny_process_write(p, code);

    return NULL;
}

int main()
{
    char *args[] = {"python3", "-u", "main.py", NULL};
    Process p = monny_process_spawn(args);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, reader, &p);
    pthread_create(&t2, NULL, writer, &p);

    pthread_join(t2, NULL);
    pthread_join(t1, NULL);

    monny_process_end(&p);
}
