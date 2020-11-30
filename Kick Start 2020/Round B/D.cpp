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

// Types
template <typename T> using vector1D = vector<T>;
template <typename T> using vector2D = vector<vector1D<T>>;
template <typename T> using vector3D = vector<vector2D<T>>;
template <typename T> using uset = unordered_set<T>;
template <typename K, typename V> using umap = unordered_map<K, V>;

// Output stream overloads.
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
    for (int i = 0; i < v.size(); ++i) {
        if (i == 0) {
            out << '[';
        }
        out << v[i];
        if (i < v.size() - 1) {
            out << ", ";
        } else {
            out << ']';
        }
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
// ______________________________________________________________________________

// long double compute(int top, int bot) {
//     cerrd << "Computing (" << top << " choose " << bot << ")\n";
//     if (top < 0 || bot < 0) {
//         return 0;
//     }
//     long double p = pow(2.0L, -(top - 1));
//     for (int i = 1; i <= top - bot; ++i) {
//         p *= top - i + 1;
//         // p /= 2 * i;
//         p /= i;
//     }
//     // for (int i = 1; i <= bot; ++i) {
//     //     p /= 2;
//     // }
//     // p *= 2;
//     return p;
// }

long double compute(const vector<long double>& factorials, int row, int col) {
    cerrd << "Compute (" << row << ", " << col << ")\n";
    return pow(2.0L, factorials[row + col - 2] - factorials[row - 1] - factorials[col - 1] - (row + col - 2));
}

void solution() {
    vector<long double> factorials(200002, 0.0L);
    for (int i = 2; i <= factorials.size() - 1; ++i) {
        factorials[i] = factorials[i - 1] + logl(i) / logl(2);
    }

    cout << fixed << setprecision(8);

    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int W, H, L, U, R, D;
        cin >> W >> H >> L >> U >> R >> D;
        long double prob = 0;
        if (H > D) {
            for (int col = L - 1; col >= 1; --col) {
                int row = D + L - col;
                prob += compute(factorials, row, col);
            }
        }
        if (W > R) {
            for (int row = U - 1; row >= 1; --row) {
                int col = R + U - row;
                prob += compute(factorials, row, col);
            }
        }
        cout << "Case #" << t << ": " << prob << '\n';
    }
}
