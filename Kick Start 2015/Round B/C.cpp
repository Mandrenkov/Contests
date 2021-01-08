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

// constexpr int MAX_F = 31622776;
constexpr int MAX_P = 150;

vector<bool> prime(MAX_P + 1, true);

umap<long long, bool> dp;

bool isSumPrime(long long n) {
    int sum = 0;
    for (long long m = n; m > 0; m /= 10) {
        sum += m % 10;
    }
    return prime[sum];
}

int isqrt(long long n) {
    int lo = 1;
    int hi = n;
    while (lo < hi) {
        long long mid = (lo + hi + 1) / 2;
        if (mid * mid <= n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

vector<int> factorize(long long n) {
    vector<int> factors;
    long long bound = isqrt(n);
    for (int i = 2; n > 1 && i <= bound; i += (i == 2) ? 1 : 2) {
        if (n % i == 0) {
            factors.emplace_back(i);
            while (n > 1 && n % i == 0) {
                n /= i;
            }
        }
    }
    return factors;
}

bool solve(long long n) {
    if (dp.find(n) != dp.end()) {
        return dp[n];
    } else if (isSumPrime(n)) {
        return dp[n] = false;
    }

    const auto factors = factorize(n);

    bool win = false;
    for (int factor : factors) {
        long long m = n;
        while (m % factor == 0) {
            m /= factor;
        }
        if (!solve(m)) {
            win = true;
            break;
        }
    }
    return dp[n] = win;
}

void solution() {
    dp[1] = false;

    for (int i = 2; i <= MAX_P; ++i) {
        if (prime[i]) {
            for (int j = i * i; j <= MAX_P; j += i) {
                prime[j] = false;
            }
        }
    }

    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        long long N;
        cin >> N;
        bool win = solve(N);
        cout << "Case #" << t << ": " << (win ? "Laurence" : "Seymour") << "\n";
    }
}
