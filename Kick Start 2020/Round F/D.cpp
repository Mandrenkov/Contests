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
template <typename T> using vector1D = vector<T>;
template <typename T> using vector2D = vector<vector1D<T>>;
template <typename T> using vector3D = vector<vector2D<T>>;
template <typename T> using uset = unordered_set<T>;
template <typename K, typename V> using umap = unordered_map<K, V>;

// Output stream overloads.
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
    if (v.size() == 0) {
        return out << "[]";
    }

    out << '[' << v[0];
    for (int i = 1; i < v.size(); ++i) {
        out << ' ' << v[i];
    }
    return out << ']';
}

template <typename T>
ostream& operator<<(ostream& out, const vector<vector<T>>& m) {
    if (m.size() == 0) {
        return out << "[]";
    }

    for (const auto& row : m) {
        out << row << '\n';
    }
    return out;
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

    vector<K> keys;
    for (const auto& p : m) {
        keys.push_back(p.first);
    }

    out << "{" << keys[0] << ": " << m.at(keys[0]);
    for (int i = 1; i < keys.size(); ++i) {
        const auto& key = keys[i];
        out << ", " << key << ": " << m.at(key);
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

    out << "{" << keys[0] << ": " << m.at(keys[0]);
    for (int i = 1; i < keys.size(); ++i) {
        const auto& key = keys[i];
        out << ", " << key << ": " << m.at(key);
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

// Initialize the execution environment and call the solution function.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    solution();
    return 0;
}

// Solution
//______________________________________________________________________________

constexpr long long MOD = 998244353LL;

string hashify(vector<int>& G) {
    string S;
    for (int count : G) {
        S += to_string(count);
        S += '|'; 
    }
    return S;
}

bool check(vector<int>& G1, vector<int>& G2) {
    int slack = 0;
    for (int i = G1.size() - 1; i > 0; --i) {
        if (G1[i] < G2[i]) {
            slack += G2[i] - G1[i];
        } else if (G1[i] > G2[i]) {
            slack -= G1[i] - G2[i];
            if (slack < 0) {
                return false;
            }
        }
    }
    return true;
}

double brute(int N, int M, vector<int>& G) {
    auto rng = mt19937(random_device{}());
    auto dis = uniform_int_distribution<int>(1, M);
    int trials = 10000;
    double rolls = 0;
    for (int i = 0; i < trials; ++i) {
        vector<int> state(M + 1, 0);
        vector<int> groups(N + 1, 0);
        groups[0] = N;
        for (int j = 0; j < N; ++j) {
            auto roll = dis(rng);
            ++rolls;
            int count = state[roll];
            --groups[count];
            ++groups[count + 1];
            if (check(groups, G)) {
                ++state[roll];
            } else {
                --j;
                ++groups[count];
                --groups[count + 1];
            }
        }
    }
    rolls /= trials;
    return rolls;
}

umap<string, double> DP;

double smart(int N, int M, vector<int>& G1, vector<int>& G2) {
    if (N == 0) {
        return 0;
    }
    string H = hashify(G1);
    if (DP.find(H) != DP.end()) {
        return DP[H];
    }
    // Rolls --> Counts
    vector<int> S(M + 1, 0);
    int k = 0;
    for (int i = 1; i < G1.size(); ++i) {
        for (int j = 0; j < G1[i]; ++j) {
            S[k] = i;
            ++k;
        }
    }
    // cerr << "S = " << S << ", G1 = " << G1 << '\n';
    double E = 0.0;
    double good = 0;
    vector<double> results(G1.size() + 1, -1);
    for (int i = 0; i < M; ++i) {
        int count = S[i];
        if (results[count] != -1) {
            if (results[count] == -2) {
            } else {
                E += results[count];
                ++good;
            }
        } else {
            --G1[count];
            ++G1[count + 1];
            // cerr << "Checking " << G1 << " vs. " << G2 << '\n';
            if (check(G1, G2)) {
                results[count] = smart(N - 1, M, G1, G2);
                E += results[count];
                ++good;
            } else {
                results[count] = -2;
            }
            ++G1[count];
            --G1[count + 1];
        }
    }
    double success = good / M;
    E *= 1.0 / good;
    // cerr << "N = " << N << '\n';
    // cerr << "E = " << E << '\n';
    // cerr << good << '\n';
    E += 1 + (1 - success) / success;
    // cerr << "Rolls until movement = " << (1 + (1 - success) / success) << '\n';
    DP[H] = E;
    return E;
}

void solution() {
    cout << fixed << setprecision(8);
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, M, K;
        cin >> N >> M >> K;
        vector<int> G(N + 1, 0);
        for (int i = 0; i < K; ++i) {
            int group;
            cin >> group;
            ++G[group];
        }
        // cout << "Case #" << t << ": " << brute(N, M, G) << '\n';
        vector<int> G1(N + 1, 0);
        G1[0] = N;
        cout << "Case #" << t << ": " << smart(N, M, G1, G) << '\n';
        DP.clear();
    }
}