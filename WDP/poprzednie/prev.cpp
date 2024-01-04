#include <bits/stdc++.h>
#include "prev.h"

using namespace std;

struct wie
{
    int poz;
    long long minw, maxw;
    shared_ptr <wie> l, p;
};

vector <shared_ptr <wie> > drzewa;
vector <long long> tab;

void dodaj(shared_ptr <wie> v, shared_ptr <wie> odp)
{
    if(v->minw == v->maxw)
        return;
    long long sr = (v->minw + v->maxw) / 2;
    if(tab[v->poz] <= sr)
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
    else
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

void pushBack(int x)
{
    shared_ptr <wie> v = make_shared <wie>();
    drzewa.push_back(v);
    tab.push_back((long long)x + INT_MAX);
    v->poz = (int)tab.size() - 1;
    v->minw = 0;
    v->maxw = 3 * (long long)INT_MAX;
    v->l = v->p = NULL;
    dodaj(v, drzewa[drzewa.size() - 2]);
}

int szukaj(shared_ptr <wie> v, long long lo, long long hi)
{
    if(v == NULL)
        return -1;
    //cout << v->minw - INT_MAX << " " << v->maxw - INT_MAX << " " << v-> poz << " " << v->l << " " << v->p << "\n" << flush;
    if(v->minw >= lo && v->maxw <= hi)
        return v->poz - 1;
    if(v->minw > hi || v->maxw < lo)
        return -1;
    return max(szukaj(v->l, lo, hi), szukaj(v->p, lo, hi));
    //cout << "\n";
}

int prevInRange(int i, int lo, int hi)
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
    return;
}