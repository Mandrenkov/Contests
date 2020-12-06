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

constexpr int MAX_N = 100;
constexpr unsigned long long MAX_K = 1000000000000000000ULL;

void solution() {
    vector2D<unsigned long long> dp(MAX_N + 2, vector<unsigned long long>(MAX_N + 2));
    dp[0][0] = 1;
    for (int j = 0; j <= MAX_N; ++j) {
        for (int i = 0; i <= MAX_N; ++i) {
            if (i > 0) {
                dp[i][j] += dp[i - 1][j];
            }
            if (j > 0 && i <= MAX_N) {
                dp[i][j] += dp[i + 1][j - 1];
            }
            if (dp[i][j] >= MAX_K) {
                break;
            }
        }
    }

    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        unsigned long long K;
        cin >> N >> K;

        if (dp[0][N] != 0 && dp[0][N] < K) {
            cout << "Case #" << t << ": Doesn't Exist!\n";
            continue;
        }

        int level = 0;
        int limbo = N;
        string parens;
        for (int i = 0; i < 2 * N; ++i) {
            if (limbo > 0 && (level == 0 || dp[level + 1][limbo - 1] == 0 || K <= dp[level + 1][limbo - 1])) {
                --limbo;
                ++level;
                parens += '(';
            } else {
                K -= limbo == 0 ? 0 : dp[level + 1][limbo - 1];
                --level;
                parens += ')';
            }
        }
        cout << "Case #" << t << ": " << parens << "\n";
    }
}
