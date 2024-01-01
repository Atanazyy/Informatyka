#include <bits/stdc++.h>
#include "prev.h"

using namespace std;

mt19937 r;

int rr(int a, int b)
{
    return (int)(a + r() % (b - a + 1));
}

int znak()
{
    if(rr(0, 1) == 0)
        return -1;
    return 1;
}

vector <int> X;

int main()
{
    int s;
    cin >> s;
    r.seed(s);
    int n = rr(1, 1000);
    int q = rr(1, 1000);
    const int maxval = INT_MAX - 1;
    for(int i = 0; i < n; i++)
        X.push_back(rr(0, maxval) * znak());
    /*for(auto x : X)
            cout << x << " ";
    cout << "\n" << flush;*/
    init(X);
    while(q--)
    {
        /*for(auto x : X)
            cout << x << " ";
        cout << "\n" << flush;*/
        int z = znak();
        if(z == -1)
        {
            X.push_back(rr(0, maxval) * znak());
            pushBack(X.back());
        }
        else
        {
            int ind = rr(0, (int)X.size() - 1);
            int lo = rr(0, maxval) * znak(), hi = rr(0, maxval) * znak();
            if(lo > hi)
                swap(lo, hi);
            int wyn = -1;
            for(int i = 0; i <= ind; i++)
                if(X[i] >= lo && X[i] <= hi)
                    wyn = i;
            cout << wyn << " " << flush;
            //cout << "# " << ind << " " << lo << " " << hi << " " << wyn << " " << prevInRange(ind, lo, hi) << "\n" << flush;
            assert(wyn == prevInRange(ind, lo, hi));
        }
    }
    X.clear();
    done();
    cout << "\n";
}