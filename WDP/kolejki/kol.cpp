#include <bits/stdc++.h>
#include "kol.h"

using namespace std;

typedef struct lista //kazda kolejka bedzie skladala sie ze wskaznika na pierwszy i ostatni element, sa to sztuczne elementy
{
    interesant *poc, *kon;
} lista;

vector <lista> tab; //wszystkie kolejki
int nr; //numerek nastepnego interesanta

lista stworz() //tworzy nowa kolejke
{
    lista wyn;
    wyn.poc = (interesant *)malloc(sizeof(interesant)); //tworzenie sztucznego interesanta bedacego poczatkiem
    wyn.kon = (interesant *)malloc(sizeof(interesant)); //tworzenie sztucznego interesanta bedacego koncem
    wyn.poc->sas1 = NULL;
    wyn.poc->sas2 = wyn.kon;
    wyn.kon->sas1 = NULL;
    wyn.kon->sas2 = wyn.poc;
    return wyn;
}

void norm(interesant *x, interesant *y) //zamienia wskazniki sasiadow x, tak aby na sas2 byl wskaznik na y
{
    if(x->sas1 == y)
        swap(x->sas1, x->sas2);
}

void dodaj(int k, interesant *x) //dodaje intersanta x na koniec kolejki k
{
    interesant *przed = tab[k].kon->sas2; //interesant stajacy na koncu k
    norm(przed, tab[k].kon);
    przed->sas2 = tab[k].kon->sas2 = x;
    x->sas1 = przed;
    x->sas2 = tab[k].kon;
}

bool pusta(int k) //sprawdza czy kolejka jest pusta
{
    return tab[k].poc->sas2 == tab[k].kon;
}

void otwarcie_urzedu(int m) //otwiera urzad
{
    nr = 0;
    for(int i = 0; i <= m; i++)
        tab.push_back(stworz());
}

interesant *nowy_interesant(int k) //przychodzi interesant na koniec kolejki k
{
    interesant *nowy = (interesant*)malloc(sizeof(interesant));
    nowy->num = nr++;
    dodaj(k, nowy);
    return nowy;
}

int numerek(interesant *x) //zwraca numerek interesanta x
{
    return x->num;
}

interesant *obsluz(int k) //zwraca wskaznik na pierwszego interesanta w kolejce k i wyrzuca go z tej kolejki
{   
    if(pusta(k))
        return NULL;
    interesant *pie = tab[k].poc->sas2; //interesant ktorego nalezy obsluzyc
    norm(pie, tab[k].poc);
    interesant *dru = pie->sas1; //interesant ktory byl drugi a teraz bedzie pierwszy
    tab[k].poc->sas2 = dru;
    norm(dru, pie);
    dru->sas2 = tab[k].poc;
    return pie;
}

void zmiana_okienka(interesant *i, int k) //przenosi interesanta do innej kolejki
{
    interesant *obok1 = i->sas1; //interesant ktory byl z jednej strony i
    interesant *obok2 = i->sas2; //interesant ktory byl z drugiej strony i 
    norm(obok1, i);
    norm(obok2, i);
    obok1->sas2 = obok2;
    obok2->sas2 = obok1;
    dodaj(k, i);
}

void zamkniecie_okienka(int k1, int k2) //przenosi wszystkich z okienka k1 na koniec k2
{
    if(pusta(k1))
        return;
    interesant *ost2 = tab[k2].kon->sas2; //ostatni interesant w kolejce k2
    interesant *pie1 = tab[k1].poc->sas2; //pierwszy interesant w kolejce k1
    norm(ost2, tab[k2].kon);
    norm(pie1, tab[k1].poc);
    pie1->sas2 = ost2;
    ost2->sas2 = pie1;
    interesant *ost1 = tab[k1].kon->sas2; //ostatni interesant w kolejce k1, teraz bedzie tez ostatnim w kolejce k2
    norm(ost1, tab[k1].kon);
    ost1->sas2 = tab[k2].kon;
    tab[k2].kon->sas2 = ost1;
    tab[k1].poc->sas2 = tab[k1].kon;
    tab[k1].kon->sas2 = tab[k1].poc;
}

void naczelnik(int k) //odwraca kolejke
{
    if(pusta(k) || tab[k].poc->sas2 == tab[k].kon->sas2)
        return;
    interesant *pie = tab[k].poc->sas2; //pierwszy interesant, teraz bedzie ostatnim
    norm(pie, tab[k].poc);
    interesant *ost = tab[k].kon->sas2; //ostatni interesant, teraz bedzie pierwszym
    norm(ost, tab[k].kon);
    pie->sas2 = tab[k].kon;
    ost->sas2 = tab[k].poc;
    tab[k].poc->sas2 = ost;
    tab[k].kon->sas2 = pie;
}

vector <interesant *> fast_track(interesant *i1, interesant *i2) //obsluguje natychmiast interesantow od i1 do i2
{
    //nie wiemy w ktora strone od i1 jest i2, wiec bedziemy szli w dwie strony
    vector <interesant *> vec1, vec2; 
    interesant *akt1 = i1; //tym wskaznikiem bedziemy isc w jedna strone
    interesant *akt2 = i1; //tym wskaznikiem bedziemy isc w druga strone
    interesant *pop1 = i1->sas1; //wskaznik na poprzednio obsluzonego, gdy idziemy akt1
    interesant *pop2 = i1->sas2; //wskaznik na poprzednio obsluzonego, gdy idziemy akt2
    interesant *pie1 = pop1; //sasiad i1, ktory nie zostanie obsluzony, jezeli okaze sie ze trzeba bylo isc akt1
    interesant *pie2 = pop2; //sasiad i1, ktory nie zostanie obsluzony, jezeli okaze sie ze trzeba bylo isc akt2
    while(akt1 != i2 && akt2 != i2)
    {
        vec1.push_back(akt1);
        vec2.push_back(akt2);
        norm(akt1, pop1);
        if(akt1->sas1 != NULL)
        {
            pop1 = akt1;
            akt1 = akt1->sas1;
        }
        norm(akt2, pop2);
        if(akt2->sas1 != NULL)
        {
            pop2 = akt2;
            akt2 = akt2->sas1;
        }
    }
    if(akt2 == i2) //od teraz bedzie wiadomo, ze interesuje nas pierwsza opcja, czyli zmienne ***1
    {
        swap(akt1, akt2);
        swap(pie1, pie2);
        swap(pop1, pop2);
        swap(vec1, vec2);
    }
    norm(akt1, pop1);
    interesant *ost1 = akt1->sas1; //sasiad i2, ktory nie zostanie obsluzony
    norm(ost1, i2);
    norm(pie1, i1);
    pie1->sas2 = ost1;
    ost1->sas2 = pie1;
    vec1.push_back(akt1);
    return vec1;
}

vector <interesant *> zamkniecie_urzedu() //zamyka urzad
{
    vector <interesant *> wyn;
    for(auto kol : tab)
    {
        interesant *akt = kol.poc->sas2; //aktualnie rozpatrywany interesant
        interesant *pop = kol.poc; //poprzedni rozpatrzony interesant
        while(akt != kol.kon)
        {
            wyn.push_back(akt);
            norm(akt, pop);
            pop = akt;
            akt = akt->sas1;
        }
        free(kol.poc); //zwalnianie pamieci po sztucznych interesantach
        free(kol.kon); //zwalnianie pamieci po sztucznych interesantach
    }
    return wyn;
}