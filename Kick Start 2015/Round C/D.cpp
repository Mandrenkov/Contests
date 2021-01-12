#include <bits/stdc++.h>

using namespace std;

// Macros
#ifdef D3BUG
    #define cerrd if (D3BUG) cerr
#else
    #define cerrd if (false) cerr
#endif

// Types
template<class T> using vector1D = vector<T>;
template<class T> using vector2D = vector<vector1D<T>>;
template<class T> using vector3D = vector<vector2D<T>>;
template<class T> using uset = unordered_set<T>;
template<class K, class V> using umap = unordered_map<K, V>;

// Output stream overloads.
template<
    // Output stream overload for STL containers with one explicit type parameter.
    template<class> class Range, class T,
    typename = enable_if_t<std::is_same_v<Range<T>, std::deque<T>> ||
                           std::is_same_v<Range<T>, std::forward_list<T>> ||
                           std::is_same_v<Range<T>, std::list<T>> ||
                           std::is_same_v<Range<T>, std::multiset<T>> ||
                           std::is_same_v<Range<T>, std::set<T>> ||
                           std::is_same_v<Range<T>, std::unordered_multiset<T>> ||
                           std::is_same_v<Range<T>, std::unordered_set<T>> ||
                           std::is_same_v<Range<T>, std::vector<T>>>
>
std::ostream& operator<<(std::ostream& out, const Range<T>& range) {
    constexpr auto brackets = std::is_same_v<Range<T>, vector<T>> ? "[]" : "{}";
    out << brackets[0];
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (next(it) != range.end() ? ", " : "");
    }
    return out << brackets[1];
}

template<
    // Output stream overload for STL containers with two explicit type parameters.
    template<class, class> class Range, class K, class V,
    typename = enable_if_t<std::is_same_v<Range<K, V>, std::map<K, V>> ||
                           std::is_same_v<Range<K, V>, std::multimap<K, V>> ||
                           std::is_same_v<Range<K, V>, std::unordered_map<K, V>> ||
                           std::is_same_v<Range<K, V>, std::unordered_multimap<K, V>>>
>
std::ostream& operator<<(std::ostream& out, const Range<K, V>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << it->first << ": " << it->second << (next(it) != range.end() ? ", " : "");
    }
    return out << '}';
}

template <class F, class S>
std::ostream& operator<<(std::ostream& out, const std::pair<F, S>& pair) {
    return out << '(' << pair.first << ", " << pair.second << ')';
}

// Hash Template Specializations
template<class T, class U>
struct hash<std::pair<T, U>> {
    size_t operator()(const std::pair<T, U>& pair) const noexcept {
        return hash<T>{}(pair.first) ^ (hash<U>{}(pair.second) << 1);
    }
};

void solution();

// Initalize the execution environment and call the solution function.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    solution();
}

// -----------------------------------------------------------------------------

struct Meque {
    deque<int> values;
    deque<int> indices;

    Meque() = default;

    int get() {
        return values.front();
    }

    void del(int index) {
        while (!indices.empty() && indices.front() <= index) {
            indices.pop_front();
            values.pop_front();
        }
    }

    void put(int index, int value) {
        while (!values.empty() && values.back() <= value) {
            values.pop_back();
            indices.pop_back();
        }
        values.emplace_back(value);
        indices.emplace_back(index);
    }
};

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, K, C, X;
        cin >> N >> K >> C >> X;
        vector<int> A(N), B(N);
        for (int i = 0; i < N; ++i) {
            cin >> A[i];
        }
        for (int i = 0; i < N; ++i) {
            cin >> B[i];
        }
        vector2D<int> M(N, vector<int>(N));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                M[i][j] = (A[i] * (i + 1) + B[j] * (j + 1) + C) % X;
            }
        }
        vector<Meque> columns(N);
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < K; ++i) {
                columns[j].put(i, M[i][j]);
            }
        }
        long long sum = 0;
        for (int i = 0; i <= N - K; ++i) {
            // Calculate
            vector<int> maxes(N);
            for (int j = 0; j < N; ++j) {
                maxes[j] = columns[j].get();
            }
            Meque window;
            for (int j = 0; j < K; ++j) {
                window.put(j, maxes[j]);
            }
            sum += window.get();
            for (int j = 0; j < N - K; ++j) {
                window.del(j);
                window.put(j + K, maxes[j + K]);
                sum += window.get();
            }

            // Setup
            if (i < N - K) {
                for (int j = 0; j < N; ++j) {
                    columns[j].del(i);
                    columns[j].put(i + K, M[i + K][j]);
                }
            }
        }
        cout << "Case #" << t << ": " << sum << "\n";
    }
}
