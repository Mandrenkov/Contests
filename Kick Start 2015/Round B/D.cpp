#include <bits/stdc++.h>

using namespace std;

// Macros
#ifdef D3BUG
    #define cerrd if (D3BUG) cerr
#else
    #define cerrd if (false) cerr
#endif

// Types
template<class T> using vector1D = vector<T>;
template<class T> using vector2D = vector<vector1D<T>>;
template<class T> using vector3D = vector<vector2D<T>>;
template<class T> using uset = unordered_set<T>;
template<class K, class V> using umap = unordered_map<K, V>;

// Output stream overloads.
template<
    // Output stream overload for STL containers with one explicit type parameter.
    template<class> class Range, class T,
    typename = enable_if_t<std::is_same_v<Range<T>, std::deque<T>> ||
                           std::is_same_v<Range<T>, std::forward_list<T>> ||
                           std::is_same_v<Range<T>, std::list<T>> ||
                           std::is_same_v<Range<T>, std::multiset<T>> ||
                           std::is_same_v<Range<T>, std::set<T>> ||
                           std::is_same_v<Range<T>, std::unordered_multiset<T>> ||
                           std::is_same_v<Range<T>, std::unordered_set<T>> ||
                           std::is_same_v<Range<T>, std::vector<T>>>
>
std::ostream& operator<<(std::ostream& out, const Range<T>& range) {
    constexpr auto brackets = std::is_same_v<Range<T>, vector<T>> ? "[]" : "{}";
    out << brackets[0];
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (next(it) != range.end() ? ", " : "");
    }
    return out << brackets[1];
}

template<
    // Output stream overload for STL containers with two explicit type parameters.
    template<class, class> class Range, class K, class V,
    typename = enable_if_t<std::is_same_v<Range<K, V>, std::map<K, V>> ||
                           std::is_same_v<Range<K, V>, std::multimap<K, V>> ||
                           std::is_same_v<Range<K, V>, std::unordered_map<K, V>> ||
                           std::is_same_v<Range<K, V>, std::unordered_multimap<K, V>>>
>
std::ostream& operator<<(std::ostream& out, const Range<K, V>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << it->first << ": " << it->second << (next(it) != range.end() ? ", " : "");
    }
    return out << '}';
}

template <class F, class S>
std::ostream& operator<<(std::ostream& out, const std::pair<F, S>& pair) {
    return out << '(' << pair.first << ", " << pair.second << ')';
}

// Hash Template Specializations
template<class T, class U>
struct hash<std::pair<T, U>> {
    size_t operator()(const std::pair<T, U>& pair) const noexcept {
        return hash<T>{}(pair.first) ^ (hash<U>{}(pair.second) << 1);
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

constexpr int MOD = 1E9 + 7;

using Memo = umap<char, umap<int, umap<int, umap<int, int>>>>;

int solve(const string& S, Memo& dp, char last, int n, int a, int b) {
    if (dp[last][n][a].find(b) != dp[last][n][a].end()) {
        return dp[last][n][a][b];
    } else if (n == S.size()) {
        return last == 'd' && a == 0 && b == 0;
    }

    int ways = solve(S, dp, last, n + 1, a, b);
    if ((last == 'd' || last == 'a') && S[n] == 'a') {
        if (b == 0) {
            ways += solve(S, dp, 'a', n + 1, a + 1, 0);
            ways %= MOD;
        }
    } else if ((last == 'a' || last == 'b') && S[n] == 'b') {
        if (a > 0) {
            ways += solve(S, dp, 'b', n + 1, a, b + 1);
            ways %= MOD;
        }
    } else if ((last == 'b' || last == 'c') && S[n] == 'c') {
        if (a > 0 && b > 0) {
            ways += solve(S, dp, 'c', n + 1, a - 1, b);
            ways %= MOD;
        }
    } else if ((last == 'c' || last == 'd') && S[n] == 'd') {
        if (a == 0 && b > 0) {
            ways += solve(S, dp, 'd', n + 1, 0, b - 1);
            ways %= MOD;
        }
    }
    return dp[last][n][a][b] = ways;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        string S;
        cin >> S;
        int N = S.size();
        Memo dp;
        cout << "Case #" << t << ": " << solve(S, dp, 'd', 0, 0, 0) - 1 << "\n";
    }
}
