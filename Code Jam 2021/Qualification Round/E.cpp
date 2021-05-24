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

    try {
        solution();
    } catch (const char* what) {
        cerr << "Exception: " << what << ".\n";
    }
}

// -----------------------------------------------------------------------------

constexpr int Q = 10000;
constexpr int P = 100;

void solution() {
    int T, X;
    cin >> T >> X;
    for (int t = 1; t <= T; ++t) {
        vector<bitset<Q>> scores(P);

        for (int i = 0; i < P; ++i) {
            string player;
            cin >> player;
            for (int j = 0; j < Q; ++j) {
                scores[i][j] = player[j] == '1';
            }
        }

        // Estimate the difficulty level of each question by the proportion of
        // players who answered it incorrectly.
        vector<double> q_skill(Q);
        for (int i = 0; i < Q; ++i) {
            for (int j = 0; j < P; ++j) {
                q_skill[i] += 1 - scores[j][i];
            }
            q_skill[i] /= P;
            q_skill[i] = -log(1 / q_skill[i] - 1);
        }

        // Estimate the skill level of each player by the proportion of
        // questions they answered incorrectly.
        vector<double> p_skill(P);
        for (int i = 0; i < P; ++i) {
            for (int j = 0; j < Q; ++j) {
                p_skill[i] += scores[i][j];
            }
            p_skill[i] /= Q;
            p_skill[i] = -log(1 / p_skill[i] - 1);
        }

        // Sort the question difficulties to extract percentiles.
        vector<double> q_skill_ordered = q_skill;
        sort(q_skill_ordered.begin(), q_skill_ordered.end());

        // Find the cheater by examining the performance of each player in the
        // extreme questions: the cheater should perform relatively poorly on
        // the easy questions and relatively well on the hard questions.
        vector<double> diff(P);
        for (int i = 0; i < P; ++i) {
            double have = 0;
            double want = 0;
            for (int j = 0; j < Q; ++j) {
                const bool lo = q_skill[j] <= q_skill_ordered[Q / 20];
                const bool hi = q_skill[j] >= q_skill_ordered[19 * Q / 20];
                if (lo || hi) {
                    double x = p_skill[i] - q_skill[j];
                    double p = 1 / (1 + exp(-x));
                    want += p;
                    have += scores[i][j];
                }
            }
            diff[i] = abs(want - have);
        }

        auto it = max_element(diff.begin(), diff.end());
        int cheater = distance(diff.begin(), it) + 1;

        cout << "Case #" << t << ": " << cheater << "\n";
    }
}
