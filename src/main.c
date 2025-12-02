#include "monny/colors.h"

int main(int argc, char **argv)
{
    monny_output_bold(stdout);
    monny_output_red(stdout);
    printf("Hello, World!\n");
    monny_output_reset(stdout);

    return 0;
}