#include <bits/stdc++.h>
#include "wys.h"

using namespace std;

map <vector<int>, pair<int, int> > stan; 
//klucz[i] to minimalna liczba klamstw, tak aby i moglobyc odpowiedzia, para to liczba pytan ktore trzeba jeszcze zadac i o co nalezy sie spytac
int n, k;

bool lisc(vector<int> vec) //sprawdza czy juz jestesmy w stanie na 100% odpowiedziec
{
    int moz = 0;
    for(int i = 1; i <= n; i++)
    {
        if(vec[i] < k + 1)
            moz++;
        if(moz > 1)
            return 0;
    }
    return 1;
}

int rozw(vector<int> vec) //zwraca w ile pytan potrzeba aby odpowiedziec i zapisuje do mapy o co nalezy pytac
{
    if(stan.find(vec) != stan.end()) //juz to bylo policzone
        return stan[vec].first;
    if(lisc(vec)) //nie potrzeba wiecej pytan
    {
        stan[vec] = {0, 0};
        return 0;
    }
    stan[vec].first = 100000;
    for(int i = 2; i <= n; i++) //sprawdzanie co sie stanie jak zadamy pytanie w i
    {
        vector<int> tak, nie;
        tak = nie = vec;
        for(int j = i; j <= n; j++)
            tak[j] = min(tak[j] + 1, k + 1);
        for(int j = 1; j < i; j++)
            nie[j] = min(nie[j] + 1, k + 1);
        int wyn = 1 + max(rozw(tak), rozw(nie));
        if(wyn <= stan[vec].first)
            stan[vec] = {wyn, i};
    }
    return stan[vec].first;
}

int main()
{
    int g;
    dajParametry(n, k, g);
    vector<int> start(n + 1, 0); //na poczatku wszystko moze by odpowiedzia przy 0 klamstwach
    rozw(start);
    for(int i = 0; i < g; i++)
    {
        vector<int> vec(n + 1, 0);
        while(!lisc(vec)) //dopoki nie znamy odpowiedzi pytamy 
        {
            int zap = stan[vec].second;
            bool odp = mniejszaNiz(zap);
            if(!odp)
                for(int j = 1; j < zap; j++)
                    vec[j] = min(vec[j] + 1, k + 1);
            else
                for(int j = zap; j <= n; j++)
                    vec[j] = min(vec[j] + 1, k + 1);
        }
        int x = 0;
        for(int j = 1; j <= n; j++)
            if(vec[j] != k + 1)
                x = j;
        odpowiedz(x);
    }
}