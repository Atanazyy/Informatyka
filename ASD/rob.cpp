#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    map<int, int> rob;
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        rob[x]++;
    }
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        rob[x]--;
    }
    vector<int> pre;
    vector<int> dis;

    int prev_pos = -1;
    for (auto r : rob) {
        int pos = r.first;
        int cnt = r.second;
        if (prev_pos != -1) {
            dis.push_back(pos - prev_pos);
            pre.push_back(cnt + pre.back());
        }
        else {
            pre.push_back(cnt);
        }
        prev_pos = pos;
    }
    dis.push_back(m - prev_pos + rob.begin()->first);

    long long sum_dist = 0;
    for (auto d : dis)
        sum_dist += d;

    vector<pair<int, int>> vec;
    for (int i = 0; i < pre.size(); i++)
        vec.push_back({pre[i], dis[i]});
    sort(vec.begin(), vec.end());

    int cut = 0;
    long long sum = 0;
    for(int i = 0; i < vec.size(); i++) {
        sum += vec[i].second;
        if(sum * 2 > sum_dist) {
            cut = i;
            break;
        }
    }

    long long res = 0;
    for(int i = 0; i < vec.size(); i++) {
        res += (long long)abs(vec[i].first - vec[cut].first) * (long long)vec[i].second;
    }
    cout << res;
}