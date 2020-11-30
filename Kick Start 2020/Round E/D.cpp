#include <bits/stdc++.h>

using namespace std;

// Common Utilities
//______________________________________________________________________________

// Macros
#ifdef D3BUG
    #define cerrd if (D3BUG) cerr
#else
    #define cerrd if (false) cerr
#endif

// Types
template <typename T>             using uset = unordered_set<T>;
template <typename K, typename V> using umap = unordered_map<K, V>;

// Output stream overloads.
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
    if (v.size() == 0) {
        return out << "[]";
    }

    out << '[';
    for (int i = 0; i < v.size(); ++i) {
        out << v[i] << (i == v.size() - 1 ? "" : ", ");
    }
    return out << ']';
}

template <typename T>
ostream& operator<<(ostream& out, const deque<T>& d) {
    if (d.size() == 0) {
        return out << "[]";
    }

    out << '[' << d[0];
    for (int i = 1; i < d.size(); ++i) {
        out << ' ' << d[i];
    }
    return out << ']';
}

template <typename T>
ostream& operator<<(ostream& out, const set<T>& s) {
    if (s.size() == 0) {
        return out << "{}";
    }

    out << '{' << *s.begin();
    for (auto it = ++s.begin(); it != s.end(); ++it) {
        out << ' ' << *it;
    }
    return out << '}';
}

template <typename T>
ostream& operator<<(ostream& out, const multiset<T>& s) {
    return out << vector<T>(s.begin(), s.end());
}

template <typename T>
ostream& operator<<(ostream& out, const unordered_set<T>& s) {
    if (s.size() == 0) {
        return out << "{}";
    }

    vector<T> v(s.begin(), s.end());
    sort(v.begin(), v.end());

    out << '{' << v[0];
    for (int i = 1; i < v.size(); ++i) {
        out << ' ' << v[i];
    }
    return out << '}';
}

template <typename K, typename V>
ostream& operator<<(ostream& out, const map<K, V>& m) {
    if (m.size() == 0) {
        return out << "{}";
    }

    out << "{\n";
    for (const auto& p : m) {
        const K& key = p.first;
        out << "    " << key << ": " << m.at(key) << '\n';
    }
    return out << '}';
}

template <typename K, typename V>
ostream& operator<<(ostream& out, const unordered_map<K, V>& m) {
    if (m.size() == 0) {
        return out << "{}";
    }

    vector<K> keys;
    for (const auto& p : m) {
        keys.push_back(p.first);
    }
    sort(keys.begin(), keys.end());

    out << "{\n";
    for (const auto& key : keys) {
        out << "    " << key << ": " << m.at(key) << '\n';
    }
    return out << '}';
}

template <typename F, typename S>
ostream& operator<<(ostream& out, const pair<F, S>& p) {
    return out << '(' << p.first << ", " << p.second << ')';
}

namespace std {
    template<typename T, typename U>
    struct hash<pair<T, U>> {
        size_t operator()(const pair<T, U>& p) const noexcept {
            return hash<T>{}(p.first) ^ (hash<U>{}(p.second) << 1);
        }
    };
}

// Forward declaration of the solution function.
void solution();

// Initalize the execution environment and call the solution function.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    solution();
    return 0;
}

// Solution
//______________________________________________________________________________

template<typename T>
using vector2D = vector<vector<T>>;

constexpr long long ENERGY_MAX = 1E12;

long long solve(int N, int S, vector2D<int>& G, vector2D<int>& S2J, vector2D<int>& R2S, vector2D<int>& S2R) {
    // vector2D<int> D(N + 1, vector<int>(N + 1, N));
    // for (int i = 1; i <= N; ++i) {
    //     D[i][i] = 0;
    // }
    // for (int i = 1; i <= N; ++i) {
    //     for (int j = 1; j <= N; ++j) {
    //         if (G[i][j]) {
    //             D[i][j] = 1;
    //         }
    //     }
    // }
    // for (int k = 1; k <= N; ++k) {
    //     for (int i = 1; i <= N; ++i) {
    //         for (int j = 1; j <= N; ++j) {
    //             D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
    //         }
    //     }
    // }
    // cerrd << "D = " << D << '\n';

    // long long best = numeric_limits<long long>::max();
    // vector<bool> visited(N + 1, false);
    // vector2D<long long> dp(S + 1, vector<long long>(N + 1));
    // for (int j = 1; j <= N; ++j) {
    //     for (int i = 0; i <= S; ++i) {
    //         for (int k = 0; k <= N; ++k) {
    //             dp[i][k] = -1;
    //         }
    //     }
    //     best = min(best, closest(dp, G, S2J, R2S, S2R, D, visited, 1, j));
    // }
    // return best < 1E12 ? best : -1;

    vector2D<long long> C(S + 1, vector<long long>(N + 1, ENERGY_MAX));
    vector2D<bool> V(S + 1, vector<bool>(N + 1, false));
    priority_queue<pair<long long, pair<int, int>>> Q;
    for (int s = 1; s <= S; ++s) {
        for (int j : S2J[s]) {
            C[s][j] = 0;
            Q.emplace(0, pair<int, int>{s, j});
        }
    }
    while (!Q.empty()) {
        const auto [priority, state] = Q.top();
        const auto cost = -priority;
        const auto [stone, junction] = state;
        Q.pop();
        if (V[stone][junction]) {
            continue;
        }
        // cerrd << "Explored " << state << '\n';
        V[stone][junction] = true;
        for (int j : G[junction]) {
            if (!V[stone][j]) {
                if (cost + 1 < C[stone][j]) {
                    C[stone][j] = cost + 1;
                    Q.emplace(-C[stone][j], pair<int, int>{stone, j});
                }
            }
        }
        for (int r : S2R[stone]) {
            // cerrd << "Trying recipe " << R2S[r] << " to make " << stone << ".\n";
            long long recost = 0;
            int product = R2S[r].back();
            if (V[product][junction]) {
                continue;
            }
            for (int i = 0; i < R2S[r].size() - 1; ++i) {
                int s2 = R2S[r][i];
                recost += C[s2][junction];
            }
            recost = min(recost, ENERGY_MAX);
            if (recost < C[product][junction]) {
                C[product][junction] = recost;
                Q.emplace(-recost, pair<int, int>{product, junction});
            }
        }
    }
    long long best = numeric_limits<long long>::max();
    for (int j = 1; j <= N; ++j) {
        best = min(best, C[1][j]);
    }
    return best < ENERGY_MAX ? best : -1;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, M, S, R;
        cin >> N >> M >> S >> R;
        vector2D<int> G(N + 1);
        for (int i = 0; i < M; ++i) {
            int u, v;
            cin >> u >> v;
            G[u].emplace_back(v);
            G[v].emplace_back(u);
        }
        cerrd << "G = " << G << '\n';

        vector2D<int> S2J(S + 1);
        for (int j = 1; j <= N; ++j) {
            int size;
            cin >> size;
            for (int i = 0; i < size; ++i) {
                int s;
                cin >> s;
                S2J[s].emplace_back(j);
            }
        }
        cerrd << "S2J = " << S2J << '\n';

        vector2D<int> R2S(R);
        vector2D<int> S2R(S + 1);
        for (int r = 0; r < R; ++r) {
            int size;
            cin >> size;
            for (int i = 0; i < size + 1; ++i) {
                int s;
                cin >> s;
                R2S[r].emplace_back(s);
                if (i < size) {
                    S2R[s].emplace_back(r);
                }
            }
            // sort(R2S[r].begin(), R2S[r].end());

            // int product;
            // cin >> product;
            // S2R[product].emplace_back(r);
        }
        cerrd << "R2S = " << R2S << '\n';
        cerrd << "S2R = " << S2R << '\n';
        long long energy = solve(N, S, G, S2J, R2S, S2R);
        cout << "Case #" << t << ": " << energy << '\n';
    }
}