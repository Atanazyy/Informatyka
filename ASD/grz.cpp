#include <bits/stdc++.h>
#include <ios>

using namespace std;

const int maxNM = 1e3 + 9;
const long long MOD = 1e9;

bool grz[maxNM][maxNM];
long long dp[maxNM][maxNM][15][3];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, m, k, g;
    cin >> n >> m >> k >> g;
    for (int i = 1; i <= g; i++) {
        int x, y;
        cin >> x >> y;
        grz[x][y] = 1;
    }
    dp[1][1][grz[1][1]][2] = 1;
    for (int j = 1; j <= m; j++) {

        for (int i = 1; i <= n; i++)
            for (int l = 0; l <= k; l++) {
                int gnum = min(k, l + grz[i + 1][j]);
                dp[i + 1][j][gnum][0] += dp[i][j][l][0] + dp[i][j][l][2];
                dp[i + 1][j][gnum][0] %= MOD;
            }

        for (int i = n; i >= 1; i--)
            for (int l = 0; l <= k; l++) {
                int gnum = min(k, l + grz[i - 1][j]);
                dp[i - 1][j][gnum][1] += dp[i][j][l][1] + dp[i][j][l][2];
                dp[i - 1][j][gnum][1] %= MOD;
            }

        for (int i = 1; i <= n; i++)
            for (int l = 0; l <= k; l++) {
                int gnum = min(k, l + grz[i][j + 1]);
                dp[i][j + 1][gnum][2] += dp[i][j][l][0] + dp[i][j][l][1] + dp[i][j][l][2];
                dp[i][j + 1][gnum][2] %= MOD;
            }
    }
    cout << (dp[n][m][k][0] + dp[n][m][k][1] + dp[n][m][k][2]) % MOD << "\n";

    // for (int i = 1; i <= n; i++)
    //     for (int j = 1; j <= m; j++)
    //         cout << "(" << dp[i][j][0][0] << " " << dp[i][j][0][1] << " " << dp[i][j][0][2] << ")" << (j == m ? '\n' : ' ');
    // cout << "\n";
    // for (int i = 1; i <= n; i++)
    //     for (int j = 1; j <= m; j++)
    //         cout << "(" << dp[i][j][1][0] << " " << dp[i][j][1][1] << " " << dp[i][j][1][2] << ")" << (j == m ? '\n' : ' ');
    // cout << "\n";
    // for (int i = 1; i <= n; i++)
    //     for (int j = 1; j <= m; j++)
    //         cout << "(" << dp[i][j][2][0] << " " << dp[i][j][2][1] << " " << dp[i][j][2][2] << ")" << (j == m ? '\n' : ' ');
}
