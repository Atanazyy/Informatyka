#include <bits/stdc++.h>
#include <ostream>

using namespace std;

const int maxS = 5e6;
const int roz = 1 << 20;
const int INF = 1e9 + 10;
int count_tree[maxS];
int max_tree[maxS];
pair<int, int> final[roz];

void update(int idx, int val) {
    int v = idx + roz;
    while (v > 0) {
        count_tree[v] ++;
        max_tree[v] = max(max_tree[v], val);
        v /= 2;
    }
}

int sum_count(int l, int r) {
    l += roz;
    r += roz;
    if (l == r)
        return count_tree[l];
    int res = count_tree[l] + count_tree[r];
    while (l / 2 != r / 2) {
        if (l % 2 == 0)
            res += count_tree[l + 1];
        if (r % 2 == 1) 
            res += count_tree[r - 1];
        l /= 2;
        r /= 2;
    }
    return res;
}

int find_first(int l, int r, int minv) {
    l += roz;
    r += roz;
    if (l == r) {
        return l - roz;
    }
    int first = (max_tree[l] >= minv) ? l : -1;
    int last = (max_tree[r] >= minv) ? r : -1;
    while (l / 2 != r / 2 && first == -1) {
        if (l % 2 == 0 && max_tree[l + 1] >= minv) {
            first = l + 1;
        }
        if (r % 2 == 1 && max_tree[r - 1] >= minv) {
            last = r - 1;
        }
        l /= 2;
        r /= 2;
    }
    if (first == -1) {
        first = last;
    }
    int v = first;
    // cout << v << flush;
    while (v <= roz) {
        if (max_tree[2 * v] >= minv) {
            v = 2 * v;
        }
        else {
            v = 2 * v + 1;
        }
    }
    return v - roz;
}

struct event {
    int type; // 0 - add point, 1 - start, 2 - end
    int val;
    int idx;
    int l;
    int r;
    int minv;
    int maxv;

    bool operator<(const event& other) const {
        if (val == other.val) {
            if (other.type == 0)
                return false;
            if (type == 0)
                return true;
        }
        return val < other.val;
    }
};

int main() {
    for (int i = 0; i < 2 * roz + 10; i++) {
        max_tree[i] = -INF;
    }
    int n, q;
    cin >> n >> q;
    vector<event> events;
    for (int i = 1; i <= n; i++) {
        event e;
        e.type = 0;
        e.idx = i;
        cin >> e.val;
        events.push_back(e);
    }
    for (int i = 0; i < q; i++) {
        event es, ee;
        es.type = 1;
        ee.type = 2;
        es.idx = i;
        ee.idx = i;
        cin >> es.l >> es.r >> es.minv >> es.maxv;
        ee.l = es.l;
        ee.r = es.r;
        ee.minv = es.minv;
        ee.maxv = es.maxv;
        es.val = es.minv - 1;
        ee.val = es.maxv;
        events.push_back(es);
        events.push_back(ee);
    }
    sort(events.begin(), events.end());
    for (auto e : events) {
        // cout << e.type << " " << e.idx << "\n" << flush;
        if (e.type == 0) {
            update(e.idx, e.val);
        }
        if (e.type == 1) {
            final[e.idx].first -= sum_count(e.l, e.r);
        }
        if (e.type == 2) {
            final[e.idx].first += sum_count(e.l, e.r);
            if (final[e.idx].first == 0) {
                final[e.idx].second = -1;
            }
            else {
                final[e.idx].second = find_first(e.l, e.r, e.minv);
            }
        }
    }
    for (int i = 0; i < q; i++) {
        cout << final[i].second << " " << final[i].first << "\n";
    }
}