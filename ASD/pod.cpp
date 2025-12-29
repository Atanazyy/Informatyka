#include <bits/stdc++.h>
#include <ios>

using namespace std;
                                                                                                                                                                         
const int r = 1e6;
vector <int> tab[r];
int lca[r][21], gle[r];

void dfs(long long v, long long o)
{
    for(auto x : tab[v])
    {
        long long u = x;
        if(u == o) continue;
        gle[u] = gle[v] + 1;
        lca[u][0] = v;
        dfs(u, v);
    }
}

int find_lca(int a, int b)
{
    if(gle[b] < gle[a]) swap(a, b);
    for(int i = 20; i >= 0; i--)
        if(gle[lca[b][i]] >= gle[a]) b = lca[b][i];
    for(int i = 20; i >= 0; i--)
    {
        if(lca[a][i] != lca[b][i])
        {
            a = lca[a][i];
            b = lca[b][i];
        }
    }
    if(a != b) a = b = lca[a][0];
    return a;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, q;
    cin >> n >> q;
    for(long long i = 1; i < n; i++)
    {
        int a, b, c;
        cin >> a >> b;
        tab[a].push_back(b);
        tab[b].push_back(a);
    }
    lca[1][0] = 1;
    dfs(1, 1);
    for(long long i = 1; i <= 20; i++)
        for(long long j = 1; j <= n; j++)
            lca[j][i] = lca[lca[j][i - 1]][i - 1];
    while (q--)
    {
        int a, b, c;
        cin >> a >> b >> c;
        int Lab = find_lca(a, b);
        int Lac = find_lca(a, c);
        int Lbc = find_lca(b, c);
        int L = find_lca(Lab, Lbc);
        cout << gle[a] + gle[b] + gle[c] - gle[Lab] - gle[Lac] - gle[Lbc] << "\n";
    }
}