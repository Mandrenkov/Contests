#include <bits/stdc++.h>

using namespace std;

using House = vector<vector<int>>;

// Returns the height of a house.
inline int height(const House& house) {
    return house.size();
}

// Returns the width of a house.
inline int width(const House& house) {
    return house[0].size();
}

// Returns the minimum safe height of a cell based on its current height and the heights
// of the adjacent cells.
inline int search(House& house, int row, int col) {
    int val = house[row][col];

    // North
    if (row > 0) {
        val = max(val, house[row - 1][col] - 1);
    }
    // East
    if (col < width(house) - 1) {
        val = max(val, house[row][col + 1] - 1);
    }
    // South
    if (row < height(house) - 1) {
        val = max(val, house[row + 1][col] - 1);
    }
    // West
    if (col > 0) {
        val = max(val, house[row][col - 1] - 1);
    }

    return val;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int R, C;
        cin >> R >> C;

        House house(R, vector<int>(C));
        for (int i = 0; i < R; ++i) {
            for (int j = 0; j < C; ++j) {
                cin >> house[i][j];
            }
        }

        long long ans = 0;

        // After `C + R` iterations, the extent of each cell is the entire house.
        for (int k = 0; k < C + R; ++k) {
            for (int i = 0; i < R; ++i) {
                for (int j = 0; j < C; ++j) {
                    int boxes = search(house, i, j);
                    ans += boxes - house[i][j];
                    house[i][j] = boxes;
                }
            }
        }

        cout << "Case #" << t << ": " << ans << "\n";
    }
}
