#ifndef MONNY_COLORS_H
#define MONNY_COLORS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

    /* ===========================================================
       Detectar Wine (Wine sempre define essas variáveis)
       =========================================================== */
    static inline bool monny_running_on_wine(void)
    {
        return getenv("WINELOADERNOEXEC") != NULL ||
               getenv("WINELOADERDEBUG") != NULL ||
               getenv("WINEPREFIX") != NULL;
    }

/* ===========================================================
   Códigos ANSI — texto e background
   =========================================================== */

/* Estilos */
#define MONNY_ANSI_RESET "\033[0m"
#define MONNY_ANSI_BOLD "\033[1m"
#define MONNY_ANSI_UNDERLINE "\033[4m"

/* Cores de texto */
#define MONNY_ANSI_BLACK "\033[30m"
#define MONNY_ANSI_RED "\033[31m"
#define MONNY_ANSI_GREEN "\033[32m"
#define MONNY_ANSI_YELLOW "\033[33m"
#define MONNY_ANSI_BLUE "\033[34m"
#define MONNY_ANSI_MAGENTA "\033[35m"
#define MONNY_ANSI_CYAN "\033[36m"
#define MONNY_ANSI_WHITE "\033[37m"

/* Backgrounds */
#define MONNY_ANSI_BG_BLACK "\033[40m"
#define MONNY_ANSI_BG_RED "\033[41m"
#define MONNY_ANSI_BG_GREEN "\033[42m"
#define MONNY_ANSI_BG_YELLOW "\033[43m"
#define MONNY_ANSI_BG_BLUE "\033[44m"
#define MONNY_ANSI_BG_MAGENTA "\033[45m"
#define MONNY_ANSI_BG_CYAN "\033[46m"
#define MONNY_ANSI_BG_WHITE "\033[47m"

/* ===========================================================
   TTY detection
   =========================================================== */
#if defined(_WIN32)
#include <io.h>
#define monny_isatty _isatty
#define monny_fileno _fileno
#else
#include <unistd.h>
#define monny_isatty isatty
#define monny_fileno fileno
#endif

    static inline bool monny_is_tty(FILE *stream)
    {
        return monny_isatty(monny_fileno(stream));
    }

    /* ===========================================================
       Windows ANSI enabling
       =========================================================== */
    static int monny_ansi_enabled = 0; /* 0 = desconhecido, 1 = sim, -1 = não */

#if defined(_WIN32)
#include <windows.h>
#endif

    static inline void monny_enable_ansi_if_needed(void)
    {
        if (monny_ansi_enabled != 0)
            return;

#if defined(_WIN32)

        /* Wine funciona como Linux: ANSI puro */
        if (monny_running_on_wine())
        {
            monny_ansi_enabled = 1;
            return;
        }

        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        if (h == INVALID_HANDLE_VALUE)
        {
            monny_ansi_enabled = -1;
            return;
        }

        DWORD mode = 0;
        if (!GetConsoleMode(h, &mode))
        {
            monny_ansi_enabled = -1;
            return;
        }

        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        if (!SetConsoleMode(h, mode))
        {
            monny_ansi_enabled = -1;
            return;
        }

        monny_ansi_enabled = 1;

#else
    /* Linux, BSD, macOS */
    monny_ansi_enabled = 1;
#endif
    }

/* ===========================================================
   Gerador de funções
   =========================================================== */
#define MONNY_DEFINE_COLOR_FUNC(name, code)             \
    static inline FILE *monny_output_##name(FILE *s)    \
    {                                                   \
        monny_enable_ansi_if_needed();                  \
        if (monny_ansi_enabled == 1 && monny_is_tty(s)) \
            fputs(code, s);                             \
        return s;                                       \
    }

    /* Estilos */
    MONNY_DEFINE_COLOR_FUNC(reset, MONNY_ANSI_RESET)
    MONNY_DEFINE_COLOR_FUNC(bold, MONNY_ANSI_BOLD)
    MONNY_DEFINE_COLOR_FUNC(underline, MONNY_ANSI_UNDERLINE)

    /* Texto */
    MONNY_DEFINE_COLOR_FUNC(black, MONNY_ANSI_BLACK)
    MONNY_DEFINE_COLOR_FUNC(red, MONNY_ANSI_RED)
    MONNY_DEFINE_COLOR_FUNC(green, MONNY_ANSI_GREEN)
    MONNY_DEFINE_COLOR_FUNC(yellow, MONNY_ANSI_YELLOW)
    MONNY_DEFINE_COLOR_FUNC(blue, MONNY_ANSI_BLUE)
    MONNY_DEFINE_COLOR_FUNC(magenta, MONNY_ANSI_MAGENTA)
    MONNY_DEFINE_COLOR_FUNC(cyan, MONNY_ANSI_CYAN)
    MONNY_DEFINE_COLOR_FUNC(white, MONNY_ANSI_WHITE)

    /* Background */
    MONNY_DEFINE_COLOR_FUNC(bg_black, MONNY_ANSI_BG_BLACK)
    MONNY_DEFINE_COLOR_FUNC(bg_red, MONNY_ANSI_BG_RED)
    MONNY_DEFINE_COLOR_FUNC(bg_green, MONNY_ANSI_BG_GREEN)
    MONNY_DEFINE_COLOR_FUNC(bg_yellow, MONNY_ANSI_BG_YELLOW)
    MONNY_DEFINE_COLOR_FUNC(bg_blue, MONNY_ANSI_BG_BLUE)
    MONNY_DEFINE_COLOR_FUNC(bg_magenta, MONNY_ANSI_BG_MAGENTA)
    MONNY_DEFINE_COLOR_FUNC(bg_cyan, MONNY_ANSI_BG_CYAN)
    MONNY_DEFINE_COLOR_FUNC(bg_white, MONNY_ANSI_BG_WHITE)

#undef MONNY_DEFINE_COLOR_FUNC

#ifdef __cplusplus
}
#endif

#endif /* MONNY_COLORS_H */
