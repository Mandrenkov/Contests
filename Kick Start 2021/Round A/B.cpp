#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int R, C;
        cin >> R >> C;

        vector<vector<bool>> grid(R + 2, vector<bool>(C + 2));
        for (int i = 1; i <= R; ++i) {
            for (int j = 1; j <= C; ++j) {
                char cell;
                cin >> cell;
                grid[i][j] = cell == '1';
            }
        }

        vector<vector<vector<int>>> dp(4, vector<vector<int>>(R + 2, vector<int>(C + 2)));
        // North
        for (int i = 1; i <= R; ++i) {
            for (int j = 1; j <= C; ++j) {
                dp[0][i][j] = grid[i][j] ? dp[0][i - 1][j] + 1 : 0;
            }
        }
        // South
        for (int i = R; i >= 1; --i) {
            for (int j = 1; j <= C; ++j) {
                dp[2][i][j] = grid[i][j] ? dp[2][i + 1][j] + 1 : 0;
            }
        }
        // East
        for (int i = 1; i <= R; ++i) {
            for (int j = C; j >= 1; --j) {
                dp[1][i][j] = grid[i][j] ? dp[1][i][j + 1] + 1 : 0;
            }
        }
        // West
        for (int i = 1; i <= R; ++i) {
            for (int j = 1; j <= C; ++j) {
                dp[3][i][j] = grid[i][j] ? dp[3][i][j - 1] + 1 : 0;
            }
        }

        long long ans = 0;
        for (int i = 1; i <= R; ++i) {
            for (int j = 1; j <= C; ++j) {
                if (!grid[i][j]) {
                    continue;
                }

                for (int k = 0; k < 4; ++k) {
                    int legs[2] = {dp[k][i][j], dp[(k + 1) % 4][i][j]};

                    // The "L" does not extend in the current pair of directions.
                    if (legs[0] < 2 || legs[1] < 2) {
                        continue;
                    }

                    // First leg is longer.
                    ans += min(2 * legs[1], legs[0]) / 2 - 1;
                    // SEcond leg is longer.
                    ans += min(2 * legs[0], legs[1]) / 2 - 1;
                }
            }
        }

        cout << "Case #" << t << ": " << ans << "\n";
    }
}
