#include <stdio.h>
#include <stdlib.h>

#define uint unsigned int

int max(int a, int b)
{
    if(a > b)
        return a;
    return b;
}

int main()
{
    uint n;
    if(scanf("%u", &n) == 0)
        return 0;
    int* tab = (int*)malloc((n + 1) * sizeof(int));
    for(uint i = 1; i <= n; i++)
        if(scanf("%d", &tab[i]) == 0)
            return 0;
    uint i = n;
    int wyn = 1;
    while(i > 0)
    {
        uint j = i - 1;
        while(j > 0)
        {
            if(tab[j] == 0)
                break;
            if(tab[i] % tab[j]  != 0)
                break;
            j--;
        }
        wyn = max(wyn, (int)(i - j));
        i = j;
    }
    printf("%d", wyn);
}