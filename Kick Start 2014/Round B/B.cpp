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
std::ostream& operator<<(ostream& out, const std::pair<F, S>& pair) {
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

void solution() {
    cout << fixed << setprecision(8);
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int B, L, N;
        cin >> B >> L >> N;
        vector<double> prev = {750.0 * B};
        for (int level = 2; level <= L; ++level) {
            vector<double> next(level * (level + 1) / 2);
            for (int i = 0; i < prev.size(); ++i) {
                prev[i] = max(0.0, prev[i] - 250.0);
            }
            int n = 0;
            for (int r = 0; r < level - 1; ++r) {
                for (int c = 0; c <= r; ++c, ++n) {
                    double overflow = prev[n] / 3;
                    next[n] += overflow;
                    next[n + r + 1] += overflow;
                    next[n + r + 2] += overflow;
                }
            }
            prev = next;
        }
        cout << "Case #" << t << ": " << min(250.0, prev[N - 1]) << "\n";
    }
}
