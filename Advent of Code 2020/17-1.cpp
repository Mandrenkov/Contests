#include <bits/stdc++.h>

using namespace std;

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

template<
    // Output stream overload for STL containers with one explicit type parameter.
    template<class> class Range, class T,
    typename = enable_if_t<is_same_v<Range<T>, deque<T>> ||
                           is_same_v<Range<T>, forward_list<T>> ||
                           is_same_v<Range<T>, list<T>> ||
                           is_same_v<Range<T>, multiset<T>> ||
                           is_same_v<Range<T>, set<T>> ||
                           is_same_v<Range<T>, unordered_multiset<T>> ||
                           is_same_v<Range<T>, unordered_set<T>> ||
                           is_same_v<Range<T>, vector<T>>>
>
ostream& operator<<(ostream& out, const Range<T>& range) {
    constexpr auto brackets = is_same_v<Range<T>, vector<T>> ? "[]" : "{}";
    out << brackets[0];
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (next(it) != range.end() ? ", " : "");
    }
    return out << brackets[1];
}

template<
    // Output stream overload for STL containers with two explicit type parameters.
    template<class, class> class Range, class K, class V,
    typename = enable_if_t<is_same_v<Range<K, V>, map<K, V>> ||
                           is_same_v<Range<K, V>, multimap<K, V>> ||
                           is_same_v<Range<K, V>, unordered_map<K, V>> ||
                           is_same_v<Range<K, V>, unordered_multimap<K, V>>>
>
ostream& operator<<(ostream& out, const Range<K, V>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << it->first << ": " << it->second << (next(it) != range.end() ? ", " : "");
    }
    return out << '}';
}

template <class F, class S>
ostream& operator<<(ostream& out, const pair<F, S>& duo) {
    return out << '(' << duo.first << ", " << duo.second << ')';
}

template <class T, class U, class V>
ostream& operator<<(ostream& out, const tuple<T, U, V>& triple) {
    return out << '(' << get<0>(triple) << ", " << get<1>(triple) << ", " << get<2>(triple) << ')';
}

// Hash Template Specializations
template<class T, class U>
struct hash<std::pair<T, U>> {
    size_t operator()(const std::pair<T, U>& pair) const noexcept {
        return std::hash<T>{}(pair.first) ^ (std::hash<U>{}(pair.second) << 1);
    }
};

// Hash Template Specializations
template<class T, class U, class V>
struct hash<std::tuple<T, U, V>> {
    size_t operator()(const std::tuple<T, U, V>& triple) const noexcept {
        const auto h1 = std::hash<T>{}(std::get<0>(triple));
        const auto h2 = std::hash<U>{}(std::get<1>(triple));
        const auto h3 = std::hash<V>{}(std::get<2>(triple));
        return (h1 ^ (h2 << 1)) ^ (h3 << 2);
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

void solution() {
    using Cube = tuple<int, int, int>;
    unordered_set<Cube> prev;
    int N;
    cin >> N;
    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < row.size(); ++j) {
            if (row[j] == '#') {
                prev.emplace(j, i, 0);
            }
        }
    }
    for (int i = 0; i < 6; ++i) {
        unordered_set<Cube> candidates;
        for (const auto [x, y, z] : prev) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dz = -1; dz <= 1; ++dz) {
                        candidates.emplace(x + dx, y + dy, z + dz);
                    }
                }
            }
        }
        unordered_set<Cube> next;
        for (const auto& candidate : candidates) {
            const auto [x, y, z] = candidate;
            int neighbours = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dz = -1; dz <= 1; ++dz) {
                        Cube cube = {x + dx, y + dy, z + dz};
                        if (cube != candidate && prev.find(cube) != prev.end()) {
                            ++neighbours;
                        }
                    }
                }
            }
            const bool active = prev.find(candidate) != prev.end();
            if (active && 2 <= neighbours && neighbours <= 3 || !active && neighbours == 3) {
                next.emplace(candidate);
            }
        }
        prev = move(next);
    }
    cout << prev.size() << '\n';
}
