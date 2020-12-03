#include <bits/stdc++.h>

using namespace std;

// Macros
#ifdef D3BUG
    #define cerrd if (D3BUG) cerr
#else
    #define cerrd if (false) cerr
#endif

// Types
template <typename T> using vector1D = vector<T>;
template <typename T> using vector2D = vector<vector1D<T>>;
template <typename T> using vector3D = vector<vector2D<T>>;

// Output Stream Overloads
template<
    // Multiple template arguments may be needed for containers like std::map.
    template<class...> class Range, class... Ts,
    // This overload must be disabled for std::string to avoid ambiguity.
    typename = std::enable_if_t<!std::is_same_v<Range<Ts...>, std::string>>
>
std::ostream& operator<<(std::ostream& out, const Range<Ts...>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (std::next(it) != range.end() ? ", " : "");
    }
    return out << '}';
}

template <class F, class S>
ostream& operator<<(ostream& out, const pair<F, S>& pair) {
    return out << '(' << pair.first << ", " << pair.second << ')';
}

// Hash Template Specializations
template<class T, class U>
struct hash<pair<T, U>> {
    size_t operator()(const pair<T, U>& p) const noexcept {
        return hash<T>{}(p.first) ^ (hash<U>{}(p.second) << 1);
    }
};

void solution();

// Initalize the execution environment and call the solution function.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    solution();
}

// -----------------------------------------------------------------------------

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, M;
        cin >> N >> M;

        pair<int, int> S, E;
        cin >> S.first >> S.second >> E.first >> E.second;
        ++S.first;
        ++S.second;
        ++E.first;
        ++E.second;

        vector2D<int> maze(N + 2, vector<int>(M + 2, -1));
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= M; ++j) {
                cin >> maze[i][j];
            }
        }

        const vector<pair<int, int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

        // Steps, Power
        using pii = pair<int, int>;
        vector2D<pii> dp(N + 2, vector<pii>(M + 2, pii{-1, -1}));
        set<pair<pii, pii>> pq;
        pq.emplace(pii{0, -maze[S.first][S.second]}, S);
        while (!pq.empty() && dp[E.first][E.second].first == -1) {
            const auto [p1, p2] = *pq.begin();
            const auto [steps, power] = p1;
            const auto [row, col] = p2;
            pq.erase(pq.begin());
            if (dp[row][col].first != -1) {
                continue;
            }
            dp[row][col] = {steps, -power};
            for (const auto& move : moves) {
                int r = row + move.first;
                int c = col + move.second;
                if (maze[r][c] != -1 && dp[r][c].first == -1) {
                    pq.emplace(pii{steps + 1, power - maze[r][c]}, pii{r, c});
                }
            }
        }
        int power = dp[E.first][E.second].second;
        if (power == -1) {
            cout << "Case #" << t << ": Mission Impossible.\n";
        } else {
            cout << "Case #" << t << ": " << power << '\n';
        }
    }
}
