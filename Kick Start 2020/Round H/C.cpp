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

// long long distribute(long long N, const vector<long long>& sum_x, long long start, int below) {
//     long long side = N / 2;
//     if (N % 2 == 0) {
//         long long bias_L = abs(side * (side + 1) / 2 - R) + abs(side * (side - 1) / 2 - L);
//         long long bias_R = abs(side * (side - 1) / 2 - R) + abs(side * (side + 1) / 2 - L);
//         return min(bias_L, bias_R);
//     }
//     return abs(side * (side + 1) / 2 - R) + abs(side * (side + 1) / 2 - L);
// }

long long shift_x(long long N, map<long long, long long> map_x, long long start) {
    long long moves_x = 0;
    auto it = map_x.begin();
    for (long long x = start; x < start + N; ++x) {
        moves_x += abs(x - it->first);
        --(it->second);
        if (it->second == 0) {
            ++it;
        }
    }
    return moves_x;
}

void solution() {
    int T;
    cin >> T;
    for (int tc = 1; tc <= T; ++tc) {
        long long N;
        cin >> N;
        vector<pair<long long, long long>> players(N);
        for (int i = 0; i < N; ++i) {
            long long x, y;
            cin >> x >> y;
            players[i] = {x, y};
        }

        // Squish along Y.
        map<long long, long long> map_y;
        for (const auto& player : players) {
            ++map_y[player.second];
        }
        vector<long long> vec_y;
        vector<long long> cnt_y;
        for (const auto& it : map_y) {
            vec_y.emplace_back(it.first);
            cnt_y.emplace_back(it.second);
        }
        long long best_y = 0;
        for (int i = 1; i < vec_y.size(); ++i) {
            best_y += (vec_y[i] - vec_y[0]) * cnt_y[i];
        }
        long long moves_y = best_y;
        long long below_y = 0;
        long long above_y = N - cnt_y[0];
        for (int i = 1; i < vec_y.size(); ++i) {
            long long diff = vec_y[i] - vec_y[i - 1];
            below_y += cnt_y[i - 1];
            moves_y += diff * below_y;
            moves_y -= diff * above_y;
            above_y -= cnt_y[i];
            best_y = min(best_y, moves_y);
        }
        cerrd << "Squish Y = " << best_y << "\n";

        // Distribute X
        vector<long long> vec_x;
        for (const auto& player : players) {
            vec_x.emplace_back(player.first);
        }
        sort(vec_x.begin(), vec_x.end());
        vector<long long> opt_x;
        for (int i = 0; i < N; ++i) {
            opt_x.emplace_back(vec_x[i] - i);
        }
        sort(opt_x.begin(), opt_x.end());
        long long start = opt_x[N / 2];
        map<long long, long long> map_x;
        for (const auto& player : players) {
            ++map_x[player.first];
        }
        long long best_x = shift_x(N, map_x, start);
        cerrd << "Squish X = " << best_x << "\n";

        long long moves = best_y + best_x;
        cout << "Case #" << tc << ": " << moves << '\n';
        cerrd << '\n';
    }
}