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

pair<long long, bool> resolve(unordered_map<string, unordered_map<string, long long>>& G, const string& lhs, const string& rhs) {
    vector<unordered_map<string, long long>> dp(2);
    const function<bool(const string&, long long, bool)> dfs = [&](const string& var, long long sum, bool odd) -> bool {
        if (dp[odd].find(var) != dp[odd].end()) {
            return false;
        }
        dp[odd][var] = sum;
        if (var == rhs && odd) {
            return true;
        }
        for (const auto& [adj, delta] : G[var]) {
            long long adj_sum = odd ? (sum - delta) : (sum + delta);
            long long adj_odd = !odd;
            if (dfs(adj, adj_sum, adj_odd)) {
                return true;
            }
        }
        return false;
    };
    for (const auto& [adj, sum] : G[lhs]) {
        if (dfs(adj, sum, true)) {
            return {dp[1][rhs], true};
        }
    }
    return {0, false};
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;
        unordered_map<string, unordered_map<string, long long>> G;
        for (int i = 0; i < N; ++i) {
            string line;
            cin >> line;
            int j = line.find_first_of('+');
            int k = line.find_first_of('=');
            string lhs = line.substr(0, j);
            string rhs = line.substr(j + 1, k - (j + 1));
            long long sum = stoi(line.substr(k + 1));
            G[lhs][rhs] = sum;
            G[rhs][lhs] = sum;
        }
        int Q;
        cin >> Q;
        cout << "Case #" << t << ":\n";
        while (Q --> 0) {
            string line;
            cin >> line;
            int j = line.find_first_of('+');
            string lhs = line.substr(0, j);
            string rhs = line.substr(j + 1);
            const auto [sum, ok] = resolve(G, lhs, rhs);
            if (ok) {
                cout << line << '=' << sum << '\n';
            } else {
                const auto [sum_lhs, ok_lhs] = resolve(G, lhs, lhs);
                if (ok_lhs) {
                    const auto [sum_rhs, ok_rhs] = resolve(G, rhs, rhs);
                    if (ok_rhs) {
                        cout << line << '=' << (sum_lhs + sum_rhs) / 2 << '\n';
                    }
                }
            }
        }
    }
}
