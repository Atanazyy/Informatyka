#include <bits/stdc++.h>
#include "prev.h"

using namespace std;

struct wie
{
    int poz; //indeks najdalszego elementu ktory jest w przedziale [minw, maxw] 
    long long minw, maxw;
    shared_ptr <wie> l, p; //wskazniki na lewego i prawego syna lub null jezeli nie istnieje
};

vector <shared_ptr <wie> > drzewa; //wskazniki na korzenie drzew poszczegolnych elementow
vector <long long> tab; //wartosci w tablicy 

void dodaj(shared_ptr <wie> v, shared_ptr <wie> odp) //tworzy wierzcholek drzewa
{
    if(v->minw == v->maxw) //jestesmy w najnizszym mozliwym lisciu
        return;
    long long sr = (v->minw + v->maxw) / 2;
    if(tab[v->poz] <= sr) //trzeba utworzyc lewego syna
    {
        if(odp != NULL)
            v->p = odp->p;
        shared_ptr <wie> nowy = make_shared <wie>();
        v->l = nowy;
        nowy->minw = v->minw;
        nowy->maxw = sr;
        nowy->poz = v->poz;
        nowy->l = nowy->p = NULL;
        if(odp != NULL)
            dodaj(nowy, odp->l);
        else
            dodaj(nowy, NULL);
    }
    else //trzeba utworzyc prawego syna
    {
        if(odp != NULL)
            v->l = odp->l;
        shared_ptr <wie> nowy = make_shared <wie>();
        v->p = nowy;
        nowy->minw = sr + 1;
        nowy->maxw = v->maxw;
        nowy->poz = v->poz;
        nowy->l = nowy->p = NULL;
        if(odp != NULL)
            dodaj(nowy, odp->p);
        else
            dodaj(nowy, NULL);
    }
}

void pushBack(int x) //dodaje element na koniec ciagu
{
    shared_ptr <wie> v = make_shared <wie>();
    drzewa.push_back(v);
    tab.push_back((long long)x + INT_MAX); //trzymanie ujemnych liczb sprawia problemy wiec przesowam wszystko o INT_MAX
    v->poz = (int)tab.size() - 1;
    v->minw = 0;
    v->maxw = 3 * (long long)INT_MAX;
    v->l = v->p = NULL;
    dodaj(v, drzewa[drzewa.size() - 2]);
}

int szukaj(shared_ptr <wie> v, long long lo, long long hi) //zwraca indeks najdalszego elementu ktory jest w przedziale [lo,hi] o ktorym informacja jest w poddrzewie o korzeniu v
{
    if(v == NULL)
        return -1;
    if(v->minw >= lo && v->maxw <= hi)
        return v->poz - 1;
    if(v->minw > hi || v->maxw < lo)
        return -1;
    return max(szukaj(v->l, lo, hi), szukaj(v->p, lo, hi));
}

int prevInRange(int i, int lo, int hi) //zwraca najwiekszy indeks nie wiekszy niz i takiego elemntu ktory jest w przedziale [lo, hi]
{
    return szukaj(drzewa[i + 1], (long long)lo + INT_MAX, (long long)hi + INT_MAX);
}

void init(const vector<int> &vec)
{
    drzewa.push_back(NULL);
    tab.push_back(0);
    for(int x : vec)
        pushBack(x);
}

void done()
{
    drzewa.clear();
    tab.clear();
    return;
}