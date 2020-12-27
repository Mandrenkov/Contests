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

bool typeable(const vector<int>& digits, int num) {
    for (int digit = 0; num > 0; num /= 10) {
        digit = num % 10;
        if (digits[digit] == 0) {
            return false;
        }
    }
    return true;
}

int typecost(int num) {
    int cost = 0;
    while (num > 0) {
        ++cost;
        num /= 10;
    }
    return cost;
}

int findcost(const map<int, int>& literals, unordered_map<int, int>& dp, int num) {
    if (dp.find(num) != dp.end()) {
        return dp[num];
    }
    int best = -1;
    for (auto it = literals.begin(); it != literals.end() && it->first < num; ++it) {
        if (it->first > 1 && num % it->first == 0) {
            int div_cost = findcost(literals, dp, num / it->first);
            if (div_cost != -1) {
                int mul_cost = div_cost + it->second + 1;
                best = best == -1 ? mul_cost : min(best, mul_cost);
            }
        }
    }
    return dp[num] = best;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        vector<int> digits(10);
        for (int i = 0; i < 10; ++i) {
            cin >> digits[i];
        }
        int target;
        cin >> target;

        map<int, int> literals;
        for (int i = 1; i <= target; ++i) {
            if (typeable(digits, i)) {
                literals.emplace(i, typecost(i));
            }
        }

        unordered_map<int, int> dp = {{0, 0}};
        for (const auto [literal, cost] : literals) {
            dp[literal] = cost;
        }
        int cost = findcost(literals, dp, target);
        if (cost != -1) {
            cout << "Case #" << t << ": " << cost + 1 << "\n";
        } else {
            cout << "Case #" << t << ": Impossible\n";
        }
    }
}
