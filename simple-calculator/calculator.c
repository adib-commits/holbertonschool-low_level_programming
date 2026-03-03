#include <stdio.h>

int main(void)
{
    int choice;
    int a, b, result;

    printf("Simple Calculator\n");

    while (1)
    {
        printf("1) Add\n");
        printf("2) Subtract\n");
        printf("3) Multiply\n");
        printf("4) Divide\n");
        printf("0) Quit\n");

        printf("Choice: ");
        if (scanf("%d", &choice) != 1)
            return 0;

        if (choice == 0)
        {
            printf("Bye!\n");
            break;
        }
        else if (choice == 1)
        {
            printf("A: ");
            scanf("%d", &a);

            printf("B: ");
            scanf("%d", &b);

            result = a + b;
            printf("Result: %d\n", result);
        }
        else if (choice < 0 || choice > 4)
        {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
