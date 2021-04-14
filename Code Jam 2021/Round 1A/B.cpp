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

struct Card {
    long long prime;
    long long quantity;
};

// Returns the factors of the given number from the provided cards.
vector<long long> factorize(const vector<Card>& cards, long long n) {
    vector<long long> factors;
    for (auto [prime, quantity] : cards) {
        while (n % prime == 0 && quantity) {
            factors.emplace_back(prime);
            n /= prime;
            --quantity;
        }
    }
    return n == 1 ? factors : vector<long long>{};
};

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int M;
        cin >> M;

        long long sum = 0;
        vector<Card> cards(M);
        for (int i = 0; i < M; ++i) {
            cin >> cards[i].prime >> cards[i].quantity;
            sum += cards[i].prime * cards[i].quantity;
        }

        // There are at most 64 cards in the "multiplication" group.
        long long lo = max(2LL, sum - cards.back().prime * 64LL);
        long long hi = sum;

        long long ans = 0;

        for (long long target = hi; target >= lo && ans == 0; --target) {
            const auto factors = factorize(cards, target);
            if (factors.empty()) {
                continue;
            }

            long long gap = accumulate(factors.begin(), factors.end(), 0LL);
            if (gap + target == sum) {
                ans = target;
            }
        }

        cout << "Case #" << t << ": " << ans << "\n";
    }
}
