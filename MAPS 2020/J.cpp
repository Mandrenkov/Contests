#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(10);

    int N;
    cin >> N;
    vector<double> S(N + 1, 0);
    for (int i = 0; i < N; ++i) {
        cin >> S[i];
    }

    double total_score = 0;
    for (int i = 0; i < N; ++i) {
        total_score += S[i] * pow(4.0 / 5.0, i) / 5;
    }

    double total_score_L = 0;
    double total_score_R = total_score - S.front() / 5;
    double leave_score = 0;
    for (int i = 0; i < N; ++i) {
        leave_score += total_score_L + total_score_R * (5.0 / 4.0);
        total_score_L += S[i] * pow(4.0 / 5.0, i) / 5;
        total_score_R -= S[i + 1] * pow(4.0 / 5.0, i + 1) / 5;
    }
    leave_score /= N;

    cout << total_score << '\n';
    cout << leave_score << '\n';

    return 0;
}