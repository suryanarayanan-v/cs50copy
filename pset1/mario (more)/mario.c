#include <stdio.h>
#include <cs50.h>

int main()
{
    int height = get_int("Height: ");
    while (!(height > 0 && height < 9))
    {
        height = get_int("Height: ");
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = height - 1; j > i; j--)
        {
            printf(" ");
        }
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}