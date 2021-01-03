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

constexpr long long DNE = numeric_limits<long long>::max();

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, M;
        cin >> N >> M;
        using Edge = tuple<int, int, long long>;
        vector<Edge> edges;
        for (int i = 0; i < M; ++i) {
            int u, v;
            long long c;
            cin >> u >> v >> c;
            edges.emplace_back(u, v, c);
        }
        vector2D<long long> D(N, vector<long long>(N, DNE));
        for (int i = 0; i < N; ++i) {
            D[i][i] = 0;
        }
        for (const auto [u, v, cost] : edges) {
            D[u][v] = min(D[u][v], cost);
            D[v][u] = D[u][v];
        }
        for (int k = 0; k < N; ++k) {
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (D[i][k] != DNE && D[k][j] != DNE) {
                        D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
                        D[j][i] = D[i][j];
                    }
                }
            }
        }
        cout << "Case #" << t << ":\n";
        for (int i = 0; i < M; ++i) {
            const auto [u, v, cost] = edges[i];
            if (D[u][v] != cost) {
                cout << i << '\n';
            }
        }
    }
}
