//Atanazy Gawrysiak
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXN (int)1e6 + 9
#define INF (long long)1e18

typedef struct motel
{
    int siec;
    long long odl;
} motel;

motel tab[MAXN];
long long minwyn = INF;
long long maxwyn = -INF;

long long min(long long a, long long b) //liczenie minimum z long longow 
{
    return (a < b) ? a : b;
}

long long max(long long a, long long b) //liczenie maksimum z long longow
{
    return (a > b) ? a : b;
}

void akt(int A, int B, int C) //aktualzuje minimalny i maksymalny wynik dla trasy A -> B -> C 
{
    if(A == -1 || B == -1 || C == -1)
        return;
    if(tab[A].siec == tab[B].siec || tab[A].siec == tab[C].siec || tab[C].siec == tab[B].siec)
        return;
    if(tab[A].odl > tab[B].odl || tab[B].odl > tab[C].odl)
        return;
    long long odlAB = labs(tab[B].odl - tab[A].odl);
    long long odlBC = labs(tab[C].odl - tab[B].odl);
    minwyn = min(minwyn, max(odlAB, odlBC));
    maxwyn = max(maxwyn, min(odlAB, odlBC));
}

void licz(int n)
{
    int* nas = malloc((size_t)(n + 10)*sizeof(int)); //gdzie jest nastepny element rozny od tab[i]
    int* pop = malloc((size_t)(n + 10)*sizeof(int)); //gdzie jest poprzedni element rozny od tab[i]
    int* nas2 = malloc((size_t)(n + 10)*sizeof(int)); //gdzie jest nastepny element rozny od tab[i] oraz rozny od tab[nas[i]]
    int* pop2 = malloc((size_t)(n + 10)*sizeof(int)); //gdzie jest poprzedni element rozny od tab[i] oraz rozny od tab[nas[i]]
    nas[n] = pop[1] = nas2[n] = pop2[1] = -1; //gdy powyzsze nie istnieja to trzymane jest -1

    //liczenie nas i nas2
    for(int i = n - 1; i >= 1; i--)
    {
        if(tab[i].siec != tab[i + 1].siec)
            nas[i] = i + 1;
        else
            nas[i] = nas[i + 1];

        if(tab[i].siec != tab[i + 1].siec && nas[i + 1] != -1 && tab[i].siec != tab[nas[i + 1]].siec)
            nas2[i] = nas[i + 1];
        else
            nas2[i] = nas2[i + 1];
    }
    //liczenie pop i pop2
    for(int i = 2; i <= n; i++)
    {
        if(tab[i].siec != tab[i - 1].siec)
            pop[i] = i - 1;
        else
            pop[i] = pop[i - 1];

        if(tab[i].siec != tab[i - 1].siec && pop[i - 1] != -1 && tab[i].siec != tab[pop[i - 1]].siec)
            pop2[i] = pop[i - 1];
        else
            pop2[i] = pop2[i - 1];
    }

    for(int i = 2; i < n; i++) //liczenie wynikow dla sytuacji gdy i jest srodkowym hotelem
    { 
        //mozliwosci takie, aby 3 hotele byly jak najblizej siebie
        akt(pop[i], i, nas[i]);
        akt(pop2[i], i, nas[i]);
        akt(pop[i], i, nas2[i]);
        akt(pop2[i], i, nas2[i]);

        //mozliwosci takie, aby 3 hotele byly jak najdalej siebie
        akt(nas[1], i, pop[n]);
        akt(nas2[1], i, pop[n]);
        akt(nas[1], i, pop2[n]);
        akt(nas2[1], i, pop2[n]);
        akt(1, i, n);
        akt(1, i, pop[n]);
        akt(1, i, pop2[n]);
        akt(nas[1], i, n);
        akt(nas2[1], i, n);
    }
    free(pop);
    free(nas);
    free(nas2);
    free(pop2);
}

int main()
{
    int n;
    assert(scanf("%d", &n) == 1);
    for(int i = 1; i <= n; i++)
        assert(scanf("%d%lld", &tab[i].siec, &tab[i].odl) == 2);
    licz(n);
    if(minwyn == INF)
        minwyn = maxwyn = 0;
    printf("%lld %lld", minwyn, maxwyn);
}