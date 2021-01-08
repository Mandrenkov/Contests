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

struct Fraction {
    long long top;
    long long bot;

    Fraction(long long top, long long bot) : top(top), bot(bot) {}

    void reduce() {
        long long g = gcd(top, bot);
        while (g > 1) {
            top /= g;
            bot /= g;
            g = gcd(top, bot);
        }
    }

    Fraction operator*(const Fraction& fraction) const {
        return Fraction(top * fraction.top, bot * fraction.bot);
    }

    bool operator<(const Fraction& fraction) const {
        return top * fraction.bot < bot * fraction.top;
    }

    bool operator==(const Fraction& fraction) const {
        Fraction f1(top, bot);
        Fraction f2(fraction.top, fraction.bot);
        f1.reduce();
        f2.reduce();
        return f1.top == f2.top && f1.bot == f2.bot;
    }
};

ostream& operator<<(ostream& out, const Fraction& fraction) {
    return out << fraction.top << "/" << fraction.bot;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int num_P, num_E, num_T;
        cin >> num_P >> num_E >> num_T;
        vector<int> gears_P(num_P), gears_E(num_E), gears_T(num_T);
        for (int i = 0; i < num_P; ++i) {
            cin >> gears_P[i];
        }
        for (int i = 0; i < num_E; ++i) {
            cin >> gears_E[i];
        }
        for (int i = 0; i < num_T; ++i) {
            cin >> gears_T[i];
        }
        vector<Fraction> outer_fractions, inner_fractions;
        for (int i = 0; i < num_P; ++i) {
            for (int j = 0; j < num_T; ++j) {
                outer_fractions.emplace_back(gears_P[i], gears_T[j]);
            }
        }
        for (int i = 0; i < num_E; ++i) {
            for (int j = i + 1; j < num_E; ++j) {
                inner_fractions.emplace_back(gears_E[i], gears_E[j]);
                inner_fractions.emplace_back(gears_E[j], gears_E[i]);
            }
        }
        sort(outer_fractions.begin(), outer_fractions.end());
        sort(inner_fractions.begin(), inner_fractions.end());
        int M;
        cin >> M;
        cout << "Case #" << t << ":\n";
        for (int m = 0; m < M; ++m) {
            long long P, Q;
            cin >> P >> Q;
            bool ok = false;
            for (const auto& fraction : outer_fractions) {
                Fraction target(fraction.bot * P, fraction.top * Q);
                target.reduce();
                auto it = lower_bound(inner_fractions.begin(), inner_fractions.end(), target);
                if (it != inner_fractions.end() && *it == target) {
                    ok = true;
                    break;
                }
            }
            cout << (ok ? "Yes" : "No") << "\n";
        }
    }
}
