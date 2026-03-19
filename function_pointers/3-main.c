#include "3-calc.h"

/**
 * main - entry point
 */
int main(int argc, char *argv[])
{
    int a, b, result;
    int (*func)(int, int);

    if (argc != 4)
    {
        exit(98);
    }

    func = get_op_func(argv[2]);
    if (func == NULL)
    {
        exit(99);
    }

    a = atoi(argv[1]);
    b = atoi(argv[3]);

    if ((argv[2][0] == '/' || argv[2][0] == '%') && b == 0)
    {
        exit(100);
    }

    result = func(a, b);
    return (0);
}
