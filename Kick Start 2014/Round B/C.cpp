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
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, K;
        cin >> N >> K;
        vector<int> A(N);
        for (int i = 0; i < N; ++i) {
            cin >> A[i];
        }
        set<pair<int, int>> candidates;
        do {
            candidates.clear();
            for (int i = 1; i < N - 1; ++i) {
                int diff_L = A[i] - A[i - 1];
                int diff_R = A[i + 1] - A[i];
                if (diff_L == diff_R && diff_L == K) {
                    candidates.emplace(A[i], i);
                }
            }
            if (!candidates.empty()) {
                const auto [value, index] = *candidates.rbegin();
                A.erase(A.begin() + index - 1, A.begin() + index + 2);
                N -= 3;
            }
        } while (!candidates.empty());
        cout << "Case #" << t << ": " << N << "\n";
    }
}
