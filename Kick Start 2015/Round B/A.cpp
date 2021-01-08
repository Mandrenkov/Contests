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

struct Road {
    int src;
    int dst;
    vector<int> times;
};

ostream& operator<<(ostream& out, const Road& road) {
    return out << "{" << road.src << ", " << road.dst << ", " << road.times << "}";
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, M, K;
        cin >> N >> M >> K;
        vector<vector<Road>> G(N + 1);
        for (int i = 0; i < M; ++i) {
            int src, dst;
            cin >> src >> dst;
            Road road{src, dst, vector<int>(24)};
            for (int j = 0; j < 24; ++j) {
                cin >> road.times[j];
            }
            G[src].emplace_back(road);
            swap(road.src, road.dst);
            G[dst].emplace_back(road);
        }

        vector2D<int> arrivals(24, vector<int>(N + 1, -1));
        for (int start = 0; start < 24; ++start) {
            priority_queue<pair<int, int>> pq;
            pq.emplace(-start, 1);
            while (!pq.empty()) {
                const auto [priority, city] = pq.top();
                pq.pop();
                if (arrivals[start][city] != -1) {
                    continue;
                }
                arrivals[start][city] = -priority - start;
                int hour = -priority % 24;
                for (const auto& road : G[city]) {
                    if (arrivals[start][road.dst] == -1) {
                        pq.emplace(priority - road.times[hour], road.dst);
                    }
                }
            }
        }

        cout << "Case #" << t << ":";
        for (int k = 0; k < K; ++k) {
            int D, S;
            cin >> D >> S;
            cout << ' ' << arrivals[S][D];
        }
        cout << "\n";
    }
}
