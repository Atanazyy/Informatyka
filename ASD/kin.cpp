#include <bits/stdc++.h>

using namespace std;

const int MAXK = 10;
const int SIZE = 32768;
const long long MOD = 1e9;

long long tree[MAXK + 5][2 * SIZE + 10];

void update(int k, int v, long long x) {
    v = SIZE + v;
    while (v > 0) {
        tree[k][v] = (tree[k][v] + x) % MOD;
        v /= 2;
    }
}

long long sum(int k, int a, int b) {
    a += SIZE;
    b += SIZE;
    if (a == b) {
        return tree[k][a];
    }
    long long res = tree[k][a] + tree[k][b];
    while (a / 2 != b / 2) {
        if (a % 2 == 0) {
            res = (res + tree[k][a + 1]) % MOD;
        }
        if (b % 2 == 1) {
            res = (res + tree[k][b - 1]) % MOD;
        }
        a /= 2;
        b /= 2;
    }
    return res;
}

int main() {
    int n, k;
    cin >> n >> k;
    update(0, n + 1, 1);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        for (int j = k; j >= 0; j--) {
            long long prev = sum(j, x + 1, n + 1);
            update(j + 1, x, prev);
        }
    }
    cout << tree[k][1];
}