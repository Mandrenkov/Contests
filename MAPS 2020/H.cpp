#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int C, N, M;
    cin >> C >> N >> M;

    vector<int> cows(N);
    for (int i = 0; i < N; ++i) {
        cin >> cows[i];
    }

    vector<int> days(M);
    for (int i = 0; i < M; ++i) {
        cin >> days[i];
    }
    int max_day = *max_element(days.begin(), days.end());

    vector<vector<long long>> farms(max_day + 1, vector<long long>(C + 1, 0));
    for (int i = 0; i < N; ++i) {
        ++farms[0][cows[i]];
    }
    for (int day = 1; day <= max_day; ++day) {
        for (int pop = 1; pop <= C; ++pop) {
            if (2 * pop > C) {
                farms[day][pop] += 2LL * farms[day - 1][pop];
            } else {
                farms[day][2 * pop] += farms[day - 1][pop];
            }
        }
    }

    for (int i = 0; i < M; ++i) {
        int day = days[i];
        long long count = accumulate(farms[day].begin(), farms[day].end(), 0LL);
        cout << count << '\n';
    }

    return 0;
}