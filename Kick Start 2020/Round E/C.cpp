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

struct Toy {
    long long E;
    long long R;
    // long long slack;

    Toy(){};

    Toy(long long E, long long R) : E(E), R(R) {};

    bool operator<(const Toy& other) const {
        return slack(0) > other.slack(0);
    }

    long long slack(long long sum) const {
        return sum - E - R;
    }
};

pair<int, string> solve(int N, vector<Toy>& toys) {
    long long sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += toys[i].E;
    }
    long long loop = sum;
    cerrd << "Loop Sum = " << sum << '\n';
    // for (int i = 0; i < N; ++i) {
    //     toys[i].slack = sum - toys[i].E - toys[i].R;
        // cerrd << "Slack[" << i << "] = " << toys[i].slack << '\n';
    // }
    int removed = 0;
    pair<int, long long> best = {0, sum};
    priority_queue<Toy> PQ;
    for (int i = 0; i < N; ++i) {
        PQ.emplace(toys[i]);
        sum += toys[i].E;
        if (toys[i].slack(loop) >= 0) {
            if (sum > best.second) {
                best = {removed, sum};
            }
        } else {
            while (!PQ.empty() && PQ.top().slack(loop) < 0) {
                cerrd << "Removed Toy (" << PQ.top().E << ", " << PQ.top().R << ")" << '\n';
                const auto toy = PQ.top();
                PQ.pop();
                sum -= 2 * toy.E;
                loop -= toy.E;
                ++removed;
            }
        }
        cerrd << i << ": Sum = " << sum << ", Best = " << best << '\n';
    }
    if (!PQ.empty()) {
        return {removed, "INDEFINITELY"};
    }
    return {best.first, to_string(best.second)};
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;
        vector<Toy> toys(N);
        for (int i = 0; i < N; ++i) {
            long long e, r;
            cin >> e >> r;
            toys[i] = Toy(e, r);
        }
        const auto [removed, status] = solve(N, toys);
        cout << "Case #" << t << ": " << removed << ' ' << status << '\n';
    }
}