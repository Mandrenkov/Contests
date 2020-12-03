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
        int N;
        string D;
        cin >> N >> D;
        vector2D<int> grid(N, vector<int>(N));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cin >> grid[i][j];
            }
        }

        if (D == "up") {
            for (int i = 0; i < N / 2; ++i) {
                swap(grid[i], grid[N - 1 - i]);
            }
        } else if (D == "right") {
            const auto grid_copy = grid;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    grid[i][j] = grid_copy[N - 1 - j][i];
                }
            }
        } else if (D == "left") {
            const auto grid_copy = grid;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    grid[i][j] = grid_copy[j][N - 1 - i];
                }
            }
        }

        vector<int> last(N, N - 1);
        for (int j = 0; j < N; ++j) {
            for (int i = N - 2; i >= 0; --i) {
                int k = last[j];
                if (grid[i][j] == grid[k][j]) {
                    grid[k][j] += grid[i][j];
                    grid[i][j] = 0;
                    last[j] = i;
                } else if (grid[i][j] != 0) {
                    last[j] = i;
                }
            }
        }
        for (int j = 0; j < N; ++j) {
            int back = N - 1;
            for (int i = N - 1; i >= 0; --i) {
                if (grid[i][j] != 0) {
                    swap(grid[i][j], grid[back][j]);
                    --back;
                }
            }
        }

        if (D == "up") {
            for (int i = 0; i < N / 2; ++i) {
                swap(grid[i], grid[N - 1 - i]);
            }
        } else if (D == "left") {
            const auto grid_copy = grid;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    grid[i][j] = grid_copy[N - 1 - j][i];
                }
            }
        } else if (D == "right") {
            const auto grid_copy = grid;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    grid[i][j] = grid_copy[j][N - 1 - i];
                }
            }
        }

        cout << "Case #" << t << ":\n";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << grid[i][j] << ' ';
            }
            cout << '\n';
        }
    }
}
