#include <stdio.h>
#include <stdlib.h>

#define uint unsigned int
#define MAXN 1e6 + 9

typedef struct motel
{
    int siec;
    long long odl;
} motel;

const long long INF = 1e18;
motel tab[(int)MAXN];
long long minwyn = INF;
long long maxwyn = -INF;

long long min(long long a, long long b)
{
    return (a < b) ? a : b;
}

long long max(long long a, long long b)
{
    return (a > b) ? a : b;
}

void akt(int A, int B, int C)
{
    if(A == -1 || B == -1 || C == -1)
        return;
    if(tab[A].siec == tab[B].siec || tab[A].siec == tab[C].siec || tab[C].siec == tab[B].siec)
        return;
    if(tab[A].odl > tab[B].odl || tab[B].odl > tab[C].odl)
        return;
    long long odlAB = tab[B].odl - tab[A].odl;
    long long odlBC = tab[C].odl - tab[B].odl;
    minwyn = min(minwyn, max(odlAB, odlBC));
    maxwyn = max(maxwyn, min(odlAB, odlBC));
}

int main()
{
    int n;
    if(scanf("%d", &n)){}
    tab[0].siec = tab[n + 1].siec = -1;
    for(int i = 1; i <= n; i++)
        if(scanf("%d%lld", &tab[i].siec, &tab[i].odl)){}
    for(int A = 1; A <= n; A++)
        for(int B = 1; B <= n; B++)
            for(int C = 1; C <= n; C++)
                akt(A, B, C);
    if(minwyn == INF)
        minwyn = maxwyn = 0;
    printf("%lld %lld", minwyn, maxwyn);
}