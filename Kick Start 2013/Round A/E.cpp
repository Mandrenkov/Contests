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
    // Two explicit template parameters are needed for containers like std::map.
    template<class, class> class Range, class K, class V,
    // This overload must be disabled for std::string to avoid ambiguity.
    typename = enable_if_t<!is_same_v<Range<K, V>, string>>
>
ostream& operator<<(ostream& out, const Range<K, V>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (next(it) != range.end() ? ", " : "");
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
        cout << "Case #" << t << ":\n";
        int N;
        cin >> N;
        vector<string> G(N + 1);
        unordered_map<string, vector<int>> C;
        for (int i = 1; i <= N; ++i) {
            string clr;
            cin >> clr;
            G[i] = clr;
            C[clr].emplace_back(i);
        }
        int M;
        cin >> M;
        vector<unordered_map<int, long long>> T(N + 1);
        for (int i = 0; i < M; ++i) {
            int beg, end;
            long long time;
            cin >> beg >> end >> time;
            T[beg].emplace(end, time);
            T[beg][end] = min(T[beg][end], time);
        }
        // cerr << "G = " << G << '\n';
        // cerr << "C = " << C << '\n';
        // cerr << "T = " << T << '\n';
        int S;
        cin >> S;
        while (S --> 0) {
            int beg, end;
            cin >> beg >> end;
            vector<long long> distances(N + 1, -1);
            priority_queue<pair<long long, int>> pq;
            pq.emplace(0, beg);
            while (!pq.empty() && distances[end] == -1) {
                const auto [dist, room] = pq.top();
                pq.pop();
                if (distances[room] > -1) {
                    continue;
                }
                distances[room] = -dist;
                for (const auto [next, time] : T[room]) {
                    if (distances[next] == -1) {
                        pq.emplace(dist - time, next);
                    }
                }
                for (int next : C[G[room]]) {
                    if (distances[next] == -1) {
                        pq.emplace(dist, next);
                    }
                }
            }
            cout << distances[end] << '\n';
        }
    }
}