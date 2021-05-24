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
        int X, Y;
        cin >> X >> Y;

        string S;
        cin >> S;

        int N = S.size();

        // Create a DP array where the first dimension is the index in the mural
        // and the second dimension denotes whether 'C' or 'J' occupies the
        // current index.
        vector2D<int> dp(N, vector<int>(2));
        for (int i = N - 2; i >= 0; --i) {
            switch (S[i + 1]) {
                case '?':
                    dp[i][0] = min(dp[i + 1][0], X + dp[i + 1][1]);
                    dp[i][1] = min(dp[i + 1][1], Y + dp[i + 1][0]);
                    break;
                case 'C':
                    dp[i][0] = dp[i + 1][0];
                    dp[i][1] = dp[i + 1][0] + Y;
                    break;
                case 'J':
                    dp[i][0] = dp[i + 1][1] + X;
                    dp[i][1] = dp[i + 1][1];
                    break;
            }
        }

        int best;
        switch (S[0]) {
            case '?':
                best = min(dp[0][0], dp[0][1]);
                break;
            case 'C':
                best = dp[0][0];
                break;
            case 'J':
                best = dp[0][1];
                break;
        }

        cout << "Case #" << t << ": " << best << "\n";
    }
}
