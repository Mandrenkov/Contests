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
std::ostream& operator<<(ostream& out, const std::pair<F, S>& pair) {
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

constexpr long long MOD = 1E9 + 7;

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        long long M, N;
        cin >> M >> N;
        vector2D<long long> dp(N + 1, vector<long long>(M + 1));
        dp[0][0] = 1;
        for (int n = 1; n <= N; ++n) {
            for (int m = 0; m <= M; ++m) {
                dp[n][m] = (m * dp[n - 1][m - 1] % MOD + (M - m) * dp[n - 1][m] % MOD) % MOD;
            }
        }
        cout << "Case #" << t << ": " << dp[N][M] << "\n";
    }
}
