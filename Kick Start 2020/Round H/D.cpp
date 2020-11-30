#include <bits/stdc++.h>

using namespace std;

// Common Utilities
//______________________________________________________________________________

// Macros
#ifdef DEBUG
    #define cerrd if (DEBUG) cerr
#else
    #define cerrd if (false) cerr
#endif

#define print(label, var) cerrd << label << ": " << #var << " = " << var << '\n';

// Types
template <typename T>             using uset = unordered_set<T>;
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

template<typename T, typename U>
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
    return 0;
}

// Solution
// -----------------------------------------------------------------------------

void solution() {
    int T;
    cin >> T;
    for (int tc = 1; tc <= T; ++tc) {
        int N, Q;
        cin >> N >> Q;
        vector<bitset<26>> S(N);
        vector<vector<bool>> G(26, vector<bool>(26, false));
        for (int i = 0; i < N; ++i) {
            string name;
            cin >> name;
            for (int j = 0; j < name.size(); ++j) {
                S[i].set(name[j] - 'A');
            }
            for (int j = 0; j < 26; ++j) {
                if (S[i][j]) {
                    for (int k = j + 1; k < 26; ++k) {
                        if (S[i][k]) {
                            G[j][k] = true;
                            G[k][j] = true;
                        }
                    }
                }
            }
        }
        cout << "Case #" << tc << ":";
        for (int q = 0; q < Q; ++q) {
            int x, y;
            cin >> x >> y;
            --x;
            --y;
            bitset<26> V;
            queue<pair<int, int>> Q;
            for (int i = 0; i < 26; ++i) {
                if (S[x][i]) {
                    Q.emplace(2, i);
                    V.set(i);
                }
            }
            int ans = -1;
            while (!Q.empty()) {
                const auto [level, letter] = Q.front();
                Q.pop();
                if (S[y][letter]) {
                    ans = level;
                    break;
                }
                for (int i = 0; i < 26; ++i) {
                    if (G[letter][i] && !V[i]) {
                        Q.emplace(level + 1, i);
                        V[i] = true;
                    }
                }
            }
            cout << ' ' << ans;
        }
        cout << '\n';
    }
}