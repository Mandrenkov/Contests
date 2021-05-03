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

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;

        vector<int> A(N);
        for (int i = 0; i < N; ++i) {
            cin >> A[i];
        }

        // `D` holds the difference between consecutive elements in `A`.
        vector<int> D(N - 1);
        for (int i = 0; i < N - 1; ++i) {
            D[i] = A[i + 1] - A[i];
        }

        // `P` associates each difference index with a streak.
        vector<int> P(N - 1);

        // `S` holds the lengths of streaks.
        vector<int> S(1, 1);

        for (int i = 1; i < N - 1; ++i) {
            if (D[i - 1] == D[i]) {
                int streak = P[i - 1];
                P[i] = streak;
                ++S[streak];
            } else {
                P[i] = S.size();
                S.emplace_back(1);
            }
        }

        int best = *max_element(S.begin(), S.end());

        if (D[0] != D[1]) {
            // Try optimizing the first element in `A`.
            best = max(best, S[1] + 1);
        }
        if (D[N - 3] != D[N - 2]) {
            // Try optimizing the last element in `A`.
            best = max(best, S[S.size() - 2] + 1);
        }

        for (int i = 0; i < N - 2; ++i) {
            if (i > 0 && D[i - 1] != D[i]) {
                // Adjust the difference on the LHS of `A[i]` to increase the LHS streak.
                int target = D[i] - D[i - 1];
                if (D[i + 1] + target == D[i - 1]) {
                    // The LHS difference becomes the same as the RHS difference.
                    if (i < N - 3 && D[i - 1] == D[i + 2]) {
                        // Combine the streaks on the LHS the RHS.
                        best = max(best, S[P[i - 1]] + S[P[i + 2]] + 2);
                    } else {
                        best = max(best, S[P[i - 1]] + 2);
                    }
                } else {
                    best = max(best, S[P[i - 1]] + 1);
                }
            }

            if (i < N - 3 && D[i + 1] != D[i + 2]) {
                // Adjust the difference on the RHS of `A[i]` to increase the RHS streak.
                int target = D[i + 2] - D[i + 1];
                if (D[i] - target == D[i + 2]) {
                    // The RHS difference becomes the same as the LHS difference.
                    if (i > 0 && D[i - 1] == D[i + 2]) {
                        // Combine the streaks on the LHS the RHS.
                        best = max(best, S[P[i - 1]] + S[P[i + 2]] + 2);
                    } else {
                        best = max(best, S[P[i + 2]] + 2);
                    }
                } else {
                    best = max(best, S[P[i + 2]] + 1);
                }
            }
        }

        cout << "Case #" << t << ": " << best + 1 << "\n";
    }
}
