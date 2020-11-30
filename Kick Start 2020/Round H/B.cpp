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

vector<long long> pow5(19, 1);
vector<long long> pow10(19, 1);

long long solve(const string& s, int index) {
    if (index == s.size() + 1) {
        return 1;
    }
    long long digit = s[index - 1] - '0';
    bool have_even = digit % 2 == 0;
    bool want_even = index % 2 == 0;
    long long safe = want_even ? (digit + 1) / 2 : (digit / 2);
    long long ways = safe * pow5[s.size() - index];
    if (want_even == have_even) {
        ways += solve(s, index + 1);
    }
    cerrd << "solve(" << s << ", " << index << ") = " << ways << '\n';
    return ways;
}

long long solve(long long n) {
    string s = to_string(n);
    long long ways = 0;
    for (int i = 1; i < s.size(); ++i) {
        ways += pow5[i];
    }
    print("Smaller", ways);
    long long full = solve(s, 1);
    print("Full", full);
    return ways + full;
}

void solution() {
    for (int i = 1; i < pow5.size(); ++i) {
        pow5[i] = 5LL * pow5[i - 1];
    }
    for (int i = 1; i < pow10.size(); ++i) {
        pow10[i] = 10LL * pow10[i - 1];
    }

    int T;
    cin >> T;
    for (int tc = 1; tc <= T; ++tc) {
        long long L, R;
        cin >> L >> R;
        long long ways = solve(R) - solve(L - 1);
        cout << "Case #" << tc << ": " << ways << '\n';
        cerrd << '\n';
    }
}