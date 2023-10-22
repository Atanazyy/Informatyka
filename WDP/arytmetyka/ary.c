#include <math.h>
#include <stdio.h>
#include "stdbool.h"

const double EPS = 1e-10;

typedef struct wartosc 
{
  bool czyPusty;
  bool czyAnty; //jezeli prawda to przedstawia przedzial R-(poc, kon) 
  double poc, kon; 
} wartosc;

void zamien(wartosc *a, wartosc *b) //zamienia dwie wartosci
{
    wartosc _a = *a;
    wartosc _b = *b;
    *b = _a;
    *a = _b;
}

void zamienDouble(double *a, double *b) //zamienia dwa double
{
    double _a = *a;
    double _b = *b;
    *b = _a;
    *a = _b;
}

bool czyZero(double x) //sprawdza czy x to zero
{ 
    return fabs(x) < EPS; 
}

bool czyNiesk(double x) //sprawdza czy x jest nisekonczony
{
    if(isinf(x) == 0)
        return false;
    return true;
}

wartosc wszystkie() //zwraca przedzial wszytkich liczb
{
    wartosc wyn;
    wyn.czyPusty = false;
    wyn.czyAnty = false;
    wyn.poc = -HUGE_VAL;
    wyn.kon = HUGE_VAL;
    return wyn;
}

wartosc nic() //zwraca przedzial pusty
{
    wartosc wyn;
    wyn.czyPusty = true;
    wyn.czyAnty = false;
    wyn.poc = -HUGE_VAL;
    wyn.kon = HUGE_VAL;
    return wyn;
}

wartosc wartosc_dokladnosc(double x, double p) //zwraca x +- p%
{
    double del = fabs(p * x / 100);
    wartosc wyn;
    wyn.czyPusty = false;
    wyn.czyAnty = false;
    wyn.poc = x - del;
    wyn.kon = x + del;
    return wyn;
}

wartosc wartosc_od_do(double x, double y) //zwraca [x, y]
{
    wartosc wyn;
    wyn.czyPusty = false;
    wyn.czyAnty = false;
    wyn.poc = x;
    wyn.kon = y;
    return wyn;
}

wartosc wartosc_dokladna(double x) //zwraca [x, x]
{
    wartosc wyn;
    wyn.czyPusty = false;
    wyn.czyAnty = false;
    wyn.poc = x;
    wyn.kon = x;
    return wyn;
}

bool in_wartosc(wartosc w, double x) //zwraca prawde jezeli x \in w
{
    if(w.czyPusty)
        return false;
    if(w.czyAnty)
        return (x < w.poc + EPS || x > w.kon - EPS);
    return (x > w.poc - EPS && x < w.kon + EPS);
}

double min_wartosc(wartosc w) //zwraca najmniejsza wartosc w lub -HUGE_VAL jesli w jest nieograniczone z dolu lub NAN jesli w jest puste
{
    if(w.czyPusty)
        return NAN;
    if(w.czyAnty)
        return -HUGE_VAL;
    return w.poc;
}

double max_wartosc(wartosc w) //zwraca najwieksza wartosc w lub HUGE_VAL jesli w jest nieograniczone z gory lub NAN jesli w jest puste
{
    if(w.czyPusty)
        return NAN;
    if(w.czyAnty)
        return HUGE_VAL;
    return w.kon;
}

double sr_wartosc(wartosc w) //zwraca srodek przedzialu od min_wartosc do max_wartosc lub nan jezeli ktoras z tych wartosci nie jest okreslona
{
    if(w.czyAnty || w.czyPusty || (czyNiesk(w.poc) && czyNiesk(w.kon)))
        return NAN;
    return (w.poc + w.kon) / 2;
}

wartosc plus(wartosc a, wartosc b) //dodaje dwie liczby
{
    wartosc wyn;
    if(a.czyPusty || b.czyPusty)
        return nic();
    wyn.czyPusty = false;
    if(a.czyAnty && b.czyAnty)
        return wszystkie();
    if(!a.czyAnty && !b.czyAnty)
    {
        wyn.czyAnty = false;
        wyn.poc = a.poc + b.poc;
        wyn.kon = a.kon + b.kon;
        return wyn;
    }
    if(b.czyAnty) //od teraz a zawsze jest anty przedzialem, a b nie
        zamien(&a, &b);
    wyn.czyAnty = true;
    wyn.poc = a.poc + b.kon;
    wyn.kon = a.kon + b.poc;
    if(wyn.poc + EPS >= wyn.kon || czyNiesk(b.poc) || czyNiesk(b.kon))
        return wszystkie();
    wyn.czyAnty = true;
    return wyn;
}

wartosc razy(wartosc a, wartosc b) //mnozy a i b
{
    wartosc wyn;
    if(a.czyPusty || b.czyPusty)
        return nic();
    wyn.czyPusty = false;
    if(a.czyAnty && b.czyAnty) //mnozenie dwoch anty przedzialow
    {
        if(in_wartosc(a, 0) || in_wartosc(b, 0))
            return wszystkie();
        wyn.czyAnty = true;
        wyn.poc = fmax(a.poc * b.kon, a.kon * b.poc);
        wyn.kon = fmin(a.poc * b.poc, a.kon * b.kon);
        if(wyn.poc + EPS >= wyn.kon)
            return wszystkie();
        return wyn;
    }
    if(!a.czyAnty && !b.czyAnty) //mnozenie dwoch zwyklych przedzialow
    {
        if((czyZero(a.poc) && czyZero(a.kon)) || (czyZero(b.poc) && czyZero(b.kon)))
            return wartosc_dokladna(0);
        wyn.czyAnty = false;
        wyn.poc = fmin(fmin(a.poc * b.poc, a.poc * b.kon),fmin(a.kon * b.poc, a.kon * b.kon));
        wyn.kon = fmax(fmax(a.poc * b.poc, a.poc * b.kon),fmax(a.kon * b.poc, a.kon * b.kon));
        return wyn;
    }
    wyn.czyAnty = true;
    if(b.czyAnty) //od teraz a zawsze jest anty przedzialem, a b zwyklym przedzialem
        zamien(&a, &b);
    if(in_wartosc(b, 0))
        return wszystkie();
    if(b.kon < 0) // od teraz b ma tylko dodatnie liczby
    {
        zamienDouble(&b.poc, &b.kon);
        zamienDouble(&a.poc, &a.kon);
        b.poc *= -1;
        b.kon *= -1;
        a.poc *= -1;
        a.kon *= -1;
    }
    if(czyNiesk(b.kon)) //przypadek gdy b ma dowolnie duze liczby
    {
        if(a.poc >= EPS || a.kon <= -EPS)
            return wszystkie();
        if(a.poc >= -EPS)
        {
            wyn.poc = 0;
            wyn.kon = a.kon * b.poc;
            return wyn;
        }
        if(a.kon <= EPS)
        {
            wyn.kon = 0;
            wyn.poc = a.poc * b.poc;
            return wyn;
        }
        wyn.poc = a.poc * b.poc;
        wyn.kon = a.kon * b.poc;
        return wyn;
    }
    wyn.poc = fmax(a.poc * b.poc, a.poc * b.kon);
    wyn.kon = fmin(a.kon * b.poc, a.kon * b.kon);
    return wyn;
}

wartosc minus(wartosc a, wartosc b) //odejmuje od a b
{
    wartosc minus = wartosc_dokladna(-1);
    return plus(a, razy(minus, b));
}

wartosc odwrotnosc(wartosc a) //liczy odwrotnosc a
{
    wartosc wyn;
    if(a.czyPusty)
        return nic();
    if(!a.czyAnty && czyZero(a.poc) && czyZero(a.kon)) //dzielenie przez 0
        return nic();
    wyn.czyPusty = false;
    if(!a.czyAnty) //jezeli a jest zwyklym przedzialem
    {
        if(czyNiesk(a.poc) && czyNiesk(a.kon))
            return wszystkie();
        if(czyZero(a.poc))
        {
            wyn.czyAnty = false;
            wyn.poc = 1 / a.kon;
            wyn.kon = HUGE_VAL;
            return wyn;
        }
        if(czyZero(a.kon))
        {
            wyn.czyAnty = false;
            wyn.poc = -HUGE_VAL;
            wyn.kon = 1 / a.poc;
            return wyn;
        }
        if(in_wartosc(a, 0))
        {
            wyn.czyAnty = true;
            wyn.poc = 1 / a.poc;
            wyn.kon = 1 / a.kon;
            return wyn;
        }
        wyn.czyAnty = false;
        wyn.poc = 1 / a.kon;
        wyn.kon = 1 / a.poc;
        return wyn;
    }
    //a jest antyprzedzialem
    if(czyZero(a.poc))
    {
        wyn.czyAnty = false;
        wyn.poc = -HUGE_VAL;
        wyn.kon = 1 / a.kon;
        return wyn;
    }
    if(czyZero(a.kon))
    {
        wyn.czyAnty = false;
        wyn.poc = 1 / a.poc;
        wyn.kon = HUGE_VAL;
        return wyn;
    }
    if(in_wartosc(a, 0))
    {
        wyn.poc = 1 / a.kon;
        wyn.kon = 1 / a.poc;
        wyn.czyAnty = false;
        return wyn;
    }
    wyn.czyAnty = false;
    wyn.poc = 1 / a.poc;
    wyn.kon = 1 / a.kon;
    return wyn;
}

wartosc podzielic(wartosc a, wartosc b) //zwraca a / b
{
    return razy(a, odwrotnosc(b));
}