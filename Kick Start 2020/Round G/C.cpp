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

bool wrapper(vector<long long>& lock, long long N, int index, long long target) {
    long long min_value = min(target, lock[index]);
    long long max_value = max(target, lock[index]);
    long long direct = max_value - min_value;
    long long wrap = (min_value + N - max_value) % N;
    return wrap < direct;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        long long W, N;
        cin >> W >> N;
        vector<long long> lock(W);
        for (int i = 0; i < W; ++i) {
            cin >> lock[i];
            --lock[i];
        }
        sort(lock.begin(), lock.end());

        cerrd << "Lock = " << lock << '\n';

        int wrap_lo_index = 0;
        int wrap_hi_index = W;
        long long wrap_lo_sum = 0;
        long long wrap_hi_sum = 0;
        int direct_lo_count = 0;
        int direct_hi_count = 0;
        long long direct_lo_sum = 0;
        long long direct_hi_sum = 0;
        for (int i = 1; i < W; ++i) {
            if (lock[0] < lock[i]) {
                long long direct = lock[i] - lock[0];
                long long wrap = (N + lock[0] - lock[i]) % N;
                if (direct <= wrap) {
                    ++direct_hi_count;
                    direct_hi_sum += direct;
                } else {
                    wrap_hi_index = min(wrap_hi_index, i);
                    wrap_hi_sum += N - lock[i];
                }
            }
        }

        long long best = direct_hi_sum + wrap_hi_sum + (W - wrap_hi_index) * lock[0];

        cerrd << "Index = " << 0 << "\n";
        cerrd << "Wrap Indices = (" << wrap_lo_index << ", " << wrap_hi_index << "), ";
        cerrd << "Wrap Sums = (" << wrap_lo_sum << ", " << wrap_hi_sum << "), ";
        cerrd << "Direct Counts = (" << direct_lo_count << ", " << direct_hi_count << "), ";
        cerrd << "Direct Sums = (" << direct_lo_sum << ", " << direct_hi_sum << ")\n";
        cerrd << "Total = " << best << "\n\n";

        int prev_index = 0;
        int next_index = 1;
        while (next_index < W && lock[0] == lock[next_index]) {
            ++next_index;
        }
        while (next_index < W) {
            long long prev_target = lock[prev_index];
            long long prev_count = next_index - prev_index;
            long long next_target = lock[next_index];
            long long diff_indices = next_index - prev_index;
            long long diff_targets = next_target - prev_target;

            // Low wrappers.
            while (wrap_lo_index < prev_index && wrapper(lock, N, wrap_lo_index, next_target)) {
                wrap_lo_sum += lock[wrap_lo_index];
                direct_lo_sum -= prev_target - lock[wrap_lo_index];
                --direct_lo_count;
                ++wrap_lo_index;
            }

            // Low direct.
            direct_lo_sum += direct_lo_count * diff_targets;

            // Same.
            if (wrapper(lock, N, prev_index, next_target)) {
                wrap_lo_sum += diff_indices * prev_target;
                wrap_lo_index = next_index;
                direct_lo_count = 0;
                direct_lo_sum = 0;
            } else {
                direct_lo_count += diff_indices;
                direct_lo_sum += diff_indices * diff_targets;
            }

            // High direct.
            int next_next_index = next_index + 1;
            while (next_next_index < W && lock[next_index] == lock[next_next_index]) {
                ++next_next_index;
            }
            long long next_diff_indices = next_next_index - next_index;
            direct_hi_sum -= direct_hi_count * diff_targets;
            direct_hi_count -= next_diff_indices;

            // High wrappers.
            while (wrap_hi_index < W && !wrapper(lock, N, wrap_hi_index, next_target)) {
                wrap_hi_sum -= N - lock[wrap_hi_index];
                direct_hi_sum += lock[wrap_hi_index] - next_target;
                ++direct_hi_count;
                ++wrap_hi_index;
            }

            long long total = wrap_lo_sum + wrap_lo_index * (N - next_target) + 
                              wrap_hi_sum + (W - wrap_hi_index) * next_target + 
                              direct_lo_sum + direct_hi_sum;
            best = min(best, total);

            cerrd << "Index = " << prev_index << " --> " << next_index << " (" << next_target << ")\n";
            cerrd << "Wrap Indices = (" << wrap_lo_index << ", " << wrap_hi_index << "), ";
            cerrd << "Wrap Sums = (" << wrap_lo_sum << ", " << wrap_hi_sum << "), ";
            cerrd << "Direct Counts = (" << direct_lo_count << ", " << direct_hi_count << "), ";
            cerrd << "Direct Sums = (" << direct_lo_sum << ", " << direct_hi_sum << ")\n";
            cerrd << "Total = " << total << "\n\n";

            prev_index = next_index;
            next_index = next_index + 1;
            while (next_index < W && lock[prev_index] == lock[next_index]) {
                ++next_index;
            }
        }
        cout << "Case #" << t << ": " << best << '\n';
    }
}