#include <bits/stdc++.h>

using namespace std;

mt19937 r;

int rr(int a, int b)
{
    return a + r() % (b - a + 1);
}

vector<pair<int, int> > v;

int main()
{
    int s;
    cin >> s;
    r.seed(s);
    int n = rr(1, 10);
    cout << n << "\n";
    for(int i = 0; i < n; i++)
        v.push_back({rr(1, 10), rr(1,4)});
    sort(v.begin(), v.end());
    for(auto x : v)
        cout << x.second << " " << x.first << "\n";
}