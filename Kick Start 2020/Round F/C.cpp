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

struct State {
    pair<int, int> A;
    pair<int, int> B;
    string R;
};

ostream& operator<<(ostream& out, const State& state) {
    return out << "(" << state.A << ", " << state.B << ", " << state.R << ")";
}

umap<string, int> DP;

vector<pair<int, int>> sense(const int S, const State& state, const pair<int, int>& position) {
    int row = position.first;
    int col = position.second;
    vector<pair<int, int>> moves;
    moves.emplace_back(row, col - 1);
    moves.emplace_back(row, col + 1);
    if (col % 2 == 0) {
        moves.emplace_back(row + 1, col + 1);
    } else {
        moves.emplace_back(row - 1, col - 1);
    }
    vector<pair<int, int>> valid_moves;
    for (auto& move : moves) {
        int r = move.first;
        int c = move.second;
        if (r < 0 || r >= S || c < 0 || c > 2 * r) {
            continue;
        }
        char symbol = state.R[r * r + c];
        if (symbol == '.') {
            valid_moves.emplace_back(move);
        }
    }
    return valid_moves;
}

int tally(const State& state) {
    int pos = count(state.R.begin(), state.R.end(), 'A');
    int neg = count(state.R.begin(), state.R.end(), 'B');
    return pos - neg;
}

int solve(const int S, State& state, int turns, bool stuck) {
    auto A = state.A;
    auto B = state.B;
    string H = state.R + to_string(A.first * A.first + A.second) + to_string(B.first * B.first + B.second);
    if (DP.find(H) != DP.end()) {
        return DP[H];
    }
    int best;
    if (turns % 2 == 0) {
        best = -1 * S * S;
        const auto& moves = sense(S, state, A);
        if (moves.empty()) {
            if (stuck) {
                best = tally(state);
            } else {
                best = solve(S, state, turns + 1, true);
            }
        } else {
            for (auto move : moves) {
                int r = move.first;
                int c = move.second;
                int k = r * r + c;
                state.R[k] = 'A';
                state.A = move;
                best = max(best, solve(S, state, turns + 1, false));
                state.A = A;
                state.R[k] = '.';
            }
        }
    } else {
        best = S * S;
        const auto& moves = sense(S, state, B);
        if (moves.empty()) {
            if (stuck) {
                best = tally(state);
            } else {
                best = solve(S, state, turns + 1, true);
            }
        } else {
            for (auto move : moves) {
                int r = move.first;
                int c = move.second;
                int k = r * r + c;
                state.R[k] = 'B';
                state.B = move;
                best = min(best, solve(S, state, turns + 1, false));
                state.B = B;
                state.R[k] = '.';
            }
        }
    }
    DP[H] = best;
    return best;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        DP.clear();
        int S, R1, C1, R2, C2, C;
        cin >> S >> R1 >> C1 >> R2 >> C2 >> C;
        --R1;
        --C1;
        --R2;
        --C2;
        string R(S * S, '.');
        R[R1 * R1 + C1] = 'A';
        R[R2 * R2 + C2] = 'B';
        for (int i = 0; i < C; ++i) {
            int r, c;
            cin >> r >> c;
            --r;
            --c;
            R[r * r + c] = 'X';
        }
        State state{{R1, C1}, {R2, C2}, R};
        int answer = solve(S, state, 0, false);
        // cout << DP << '\n';
        cout << "Case #" << t << ": " << answer << '\n';
    }
}