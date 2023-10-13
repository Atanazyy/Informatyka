#include <stdio.h>
#include "gra.h"

void zam(int *a, int *b) //zamiana dwóch liczb
{
    int c = *a;
    *a = *b;
    *b = c;
}

int nwd(int a, int b) //liczenie największego wspólnego dzielnika
{
    if(a < b) 
        zam(&a, &b);
    if(b == 0)
        return a;
    return nwd(a % b, b);
}

int main()
{
    int a, b, g;
    printf("Podaj dwie liczby\n");
    if(scanf("%d%d", &a, &b)){};
    g = nwd(a, b);
    a /= g;
    b /= g;
    printf("Po skróceniu zostały liczby %d i %d\n", a, b);
    int nr = 0;
    while(a != 1 || b != 1)
    {
        int ruch = graj(a, b);
        if(ruch == 1)
        {
            printf("Gracz %d odejmuje od pierwszej liczby\n", (nr % 2) + 1);
            a--;
        }
        else
        {
            printf("Gracz %d odejmuje od drugiej liczby\n", (nr % 2) + 1);
            b--;
        }
        printf("Zostały liczy %d i %d\n", a, b);
        g = nwd(a, b);
        a /= g;
        b /= g;
        printf("Po skróceniu zostały liczby %d i %d\n", a, b);
        nr++;
    }
    printf("Gracz %d przegał", (nr % 2) + 1);
}