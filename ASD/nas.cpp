#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
long long tab[MAXN];
long long dp[MAXN][15][2];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    int n, k;
    cin >> n >> k;

    for (int i = 1; i <= n; i++) {
        cin >> tab[i];
    }

    for (int i = 1; i <= n; i++) {
        dp[i][0][0] = dp[i][0][1] = 0;
        for (int j = 0; j <= k; j++) {
            dp[i][0][0] = max(dp[i][0][0], dp[i - 1][j][0]);
            dp[i][0][1] = max(dp[i][0][1], dp[i - 1][j][1]);
        }
        for (int j = 1; j < k; j++) {
            dp[i][j][0] = dp[i - 1][j - 1][0] + tab[i];
            dp[i][j][1] = dp[i - 1][j - 1][1] + tab[i];
        }
        dp[i][k][1] = dp[i - 1][k - 1][0] + tab[i];
    }

    long long ans = 0;
    for (int j = 0; j <= k; j++) {
        ans = max(ans, dp[n][j][0]);
        ans = max(ans, dp[n][j][1]);
    }
    cout << ans << "\n";
    return 0;
}
