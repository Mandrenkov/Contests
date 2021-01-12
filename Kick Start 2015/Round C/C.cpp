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

int apart(int i, int j) {
    int x = i ^ j;
    int d = 0;
    for (int k = 1; k <= x; k <<= 1) {
        ++d;
    }
    return d;
}

bool backtrack(const int N, const vector2D<int>& elves, const vector<int>& order, vector<int>& taken, int index) {
    if (index == 1 << N) {
        return true;
    }
    int elf = order[index];
    for (int i = 0; i < (1 << N); ++i) {
        if (taken[i] == -1) {
            bool ok = true;
            for (int j = 0; j < (1 << N) && ok; ++j) {
                if (taken[j] != -1) {
                    int elf2 = taken[j];
                    ok = elves[elf][elf2] < apart(i, j);
                }
            }
            if (ok) {
                taken[i] = elf;
                if (backtrack(N, elves, order, taken, index + 1)) {
                    return true;
                }
                taken[i] = -1;
            }
        }
    }
    return false;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, M;
        cin >> N >> M;
        vector2D<int> elves(1 << N, vector<int>(1 << N, 0));
        for (int i = 0; i < M; ++i) {
            int elf1, K, B;
            cin >> elf1 >> K >> B;
            --elf1;
            for (int j = 0; j < B; ++j) {
                int elf2;
                cin >> elf2;
                --elf2;
                elves[elf1][elf2] = max(elves[elf1][elf2], K);
                elves[elf2][elf1] = elves[elf1][elf2];
            }
        }
        
        // Count the number of constraints on each elf.
        vector<int> constraints(1 << N);
        for (int i = 0; i < (1 << N); ++i) {
            for (int j = 0; j < (1 << N); ++j) {
                if (elves[i][j]) {
                    ++constraints[i];
                }
            }
        }

        // Order the elves in decreasing order of number of constraints.
        vector<int> order(1 << N);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return constraints[i] > constraints[j]; });

        vector<int> taken(1 << N, -1);
        bool possible = backtrack(N, elves, order, taken, 0);
        cout << "Case #" << t << ": " << (possible ? "YES" : "NO") << "\n";
    }
}
