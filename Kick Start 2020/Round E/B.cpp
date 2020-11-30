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

vector<int> solve(int N, int A, int B, int C) {
    if (N == 1) {
        return {1};
    }
    if (N == 2) {
        if (C == 1 && A == B) {
            return {};
        }
        return {B, A};
    }
    int D = N - (A + B - C);
    if (D < 0) {
        return {};
    }
    if (D > 0 && C == 1 && A - C == 0 && B - C == 0) {
        return {};
    }
    vector<int> heights(N);
    auto it = heights.begin();
    if (C > 1) {
        for (int i = 0; i < A - C; ++i) {
            *(it++) = 2;
        }
        *(it++) = 3;
        for (int i = 0; i < D; ++i) {
            *(it++) = 1;
        }
        for (int i = 0; i < C - 1; ++i) {
            *(it++) = 3;
        }
        for (int i = 0; i < B - C; ++i) {
            *(it++) = 2;
        }
        return heights;
    } else {
        for (int i = 0; i < A - C; ++i) {
            *(it++) = 2;
        }
        if (A - C > 0) {
            for (int i = 0; i < D; ++i) {
                *(it++) = 1;
            }
        }
        for (int i = 0; i < C; ++i) {
            *(it++) = 3;
        }
        if (A - C == 0) {
            for (int i = 0; i < D; ++i) {
                *(it++) = 1;
            }
        }
        for (int i = 0; i < B - C; ++i) {
            *(it++) = 2;
        }
    }
    return heights;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, A, B, C;
        cin >> N >> A >> B >> C;
        const auto answer = solve(N, A, B, C);
        if (answer.empty()) {
            cout << "Case #" << t << ": " << "IMPOSSIBLE" << '\n';
        } else {
            cout << "Case #" << t << ":";
            for (int height : answer) {
                cout << ' ' << height;
            }
            cout << '\n';
        }
    }
}