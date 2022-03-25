#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main()
{
    int cents;
    float dollars = get_float("Change owed: ");
    cents = round(dollars * 100);
    while (cents <= 0)
    {
        dollars = get_float("Change owed: ");
        cents = round(dollars * 100);
    }
    
    int c25 = cents / 25;
    int c10 = (cents % 25) / 10;
    int c5 = (cents % 25 % 10) / 5;
    int c1 = (cents % 25 % 10 % 5) / 1;
    
    printf("%d\n", c25 + c10 + c5 + c1);
}