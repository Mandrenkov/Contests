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
        cin >> N;
        bool ok = true;
        vector2D<int> grid(N * N, vector<int>(N * N));
        for (int i = 0; i < N * N; ++i) {
            for (int j = 0; j < N * N; ++j) {
                cin >> grid[i][j];
                --grid[i][j];
                ok = ok && (0 <= grid[i][j] && grid[i][j] < N * N);
            }
        }
        const auto validate = [&](const vector<int>& counters) {
            return all_of(counters.begin(), counters.end(), [](const int counter) {return counter == 1;});
        };
        vector<int> counters(N * N);
        for (int i = 0; i < N * N && ok; ++i) {
            fill(counters.begin(), counters.end(), 0);
            for (int j = 0; j < N * N; ++j) {
                ++counters[grid[i][j]];
            }
            ok = validate(counters);
        }
        for (int j = 0; j < N * N && ok; ++j) {
            fill(counters.begin(), counters.end(), 0);
            for (int i = 0; i < N * N; ++i) {
                ++counters[grid[i][j]];
            }
            ok = validate(counters);
        }
        for (int r1 = 0; r1 < N && ok; ++r1) {
            for (int c1 = 0; c1 < N && ok; ++c1) {
                fill(counters.begin(), counters.end(), 0);
                for (int i = N * r1; i < N * (r1 + 1); ++i) {
                    for (int j = N * c1; j < N * (c1 + 1); ++j) {
                        ++counters[grid[i][j]];
                    }
                }
                ok = validate(counters);
            }
        }
        cout << "Case #" << t << ": " << (ok ? "Yes" : "No") << '\n';
    }
}
