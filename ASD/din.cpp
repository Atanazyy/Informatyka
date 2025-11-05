#include <bits/stdc++.h>

using namespace std;

const long long INF = 1e12 + 5;
const int MAXK = 1e7 + 5;

long long dp[MAXK];
int nr[MAXK];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    int n, m;
    cin >> n >> m;

    for (int i = 0; i <= 10; i++) {
        nr[i] = i;
        dp[i] = INF;
    }
    dp[0] = 0;

    int last_idx = 10;
    int last_nr = 10;

    for (int i = 0; i < m; i++) {
        int x;
        cin >> x;
        if (x == n) {
            cout << -1;
            return 0;
        }
        
        int start = max(last_nr + 1, x - 10);
        for (int j = start; j <= x + 10; j++) {
            last_idx++;
            nr[last_idx] = j;
            dp[last_idx] = INF;
        }
        last_nr = x + 10;

        for (int j = last_idx; j >= 0; j--)
            if (nr[j] == x) {
                dp[j] = -1;
                break;
            }
    }

    int start = max(last_nr + 1, n - 10);
    for (int j = start; j <= n + 10; j++) {
        last_idx++;
        nr[last_idx] = j;
        dp[last_idx] = INF;
    }

    for (int i = 0; i <= last_idx; i++) {
        // cout << nr[i] << " " << dp[i] << "\n";
        if (nr[i] == n) {
            if (dp[i] == INF || dp[i] == -1) {
                cout << -1;
                return 0;
            }
            cout << dp[i];
            return 0;
        }
        if (dp[i] == -1 || dp[i] == INF) {
            continue;
        }
        if (dp[i + 1] != -1) {
            dp[i + 1] = min(dp[i + 1], nr[i + 1] - nr[i] + dp[i]);
        }
        if (dp[i + 5] != -1 && nr[i] + 5 == nr[i + 5]) {
            dp[i + 5] = min(dp[i + 5], 10 + dp[i]);
        }
        if (dp[i + 10] != -1 && nr[i] + 10 == nr[i + 10]) {
            dp[i + 10] = min(dp[i + 10], 100 + dp[i]);
        }
    }
    
}
