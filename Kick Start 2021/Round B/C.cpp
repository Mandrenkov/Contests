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

// Output Stream Overloads
template<
    // Multiple template arguments may be needed for containers like std::map.
    template<class...> class Range, class... Ts,
    // This overload must be disabled for std::string to avoid ambiguity.
    typename = std::enable_if_t<!std::is_same_v<Range<Ts...>, std::string>>
>
std::ostream& operator<<(std::ostream& out, const Range<Ts...>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (std::next(it) != range.end() ? ", " : "");
    }
    return out << '}';
}

template <class F, class S>
ostream& operator<<(ostream& out, const pair<F, S>& pair) {
    return out << '(' << pair.first << ", " << pair.second << ')';
}

// Hash Template Specializations
template<class T, class U>
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
}

// -----------------------------------------------------------------------------

using int_t = unsigned long long;

// Returns true if the given number is prime.
bool is_prime(int_t n) {
    if (n < 4) {
        return true;
    } else if (n % 2 == 0) {
        return false;
    }
    for (int_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int_t Z;
        cin >> Z;

        // Find the smallest integer `n` whose square is not less than `Z`.
        int_t n = sqrt(Z);
        while ((n + 1) * (n + 1) <= Z) {
            ++n;
        }

        // Find the smallest prime greater than `n`.
        vector<int_t> primes;
        int_t m = n + 1;
        while (!is_prime(m)) {
            ++m;
        }
        primes.emplace_back(m);

        // Find the two largest primes no greater than `n`.
        while (primes.size() < 3) {
            if (is_prime(n)) {
                primes.emplace_back(n);
            }
            --n;
        }

        // Note that `primes` is sorted in descending order.
        int_t secret = primes[0] * primes[1] <= Z ? primes[0] * primes[1]
                                                  : (primes[1] * primes[2]);
        cout << "Case #" << t << ": " << secret << "\n";
    }
}
