#include <bits/stdc++.h>
#include <ios>

using namespace std;

const int maxS = 3e6;
const int maxM = 1e6;
const int roz = 1 << 20;
int tree[maxS];
int final[maxM];

void update (int v) {
    v += roz;
    while (v > 0) {
        tree[v]++;
        v /= 2;
    }
}

int sum(int l, int r) {
    l += roz;
    r += roz;
    if (l == r)
        return tree[l];
    int res = tree[l] + tree[r];
    while (l / 2 != r / 2) {
        if (l % 2 == 0)
            res += tree[l + 1];
        if (r % 2 == 1) 
            res += tree[r - 1];
        l /= 2;
        r /= 2;
    }
    return res;
}

struct query {
    int x;
    int q; // if q = 0 add 1 to l, otherwise add q * sum(l, r) to rectangle id
    int l;
    int r;
    int id;


    bool operator<(const query& other) const {
        if (x == other.x) {
            if (other.q == 0)
                return false;
            if (q == 0)
                return true;
        }
        return x < other.x;
    }
};

struct plot {
    int xl;
    int xr;
    int yl;
    int yr;
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    set<int> ys;
    map<int, int> scale;
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> trees;
    vector<query> queries;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        ys.insert(y);
        trees.push_back({x, y});
    }

    vector<plot> plots;
    for (int i = 0; i < m; i++) {
        plot p;
        cin >> p.xl >> p.xr >> p.yl >> p.yr;
        ys.insert(p.yl);
        ys.insert(p.yr);
        plots.push_back(p);
    }
    
    int idx = 0;
    for (auto y : ys) {
        scale[y] = idx;
        idx++;
    }

    idx = 0;
    for (plot p : plots) {
        query qs, qe;
        qs.id = qe.id = idx;
        qs.q = -1;
        qe.q = 1;
        qs.x = p.xl - 1;
        qe.x = p.xr;
        qs.l = qe.l = scale[p.yl];
        qs.r = qe.r = scale[p.yr];
        idx++;
        queries.push_back(qs);
        queries.push_back(qe);
    }

    for (auto t : trees) {
        query q;
        q.q = 0;
        q.x = t.first;
        q.l = scale[t.second];
        queries.push_back(q);
    }

    sort(queries.begin(), queries.end());
    for (auto q : queries) {
        if (q.q == 0) {
            update(q.l);
        }
        else {
            final[q.id] += q.q * sum(q.l, q.r);
        }
    }

    for (int i = 0; i < m; i++) {
        cout << final[i] << "\n";
    }
}