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

        vector<string> X(N);
        for (int i = 0; i < N; ++i) {
            cin >> X[i];
        }

        int ops = 0;
        for (int i = 1; i < N; ++i) {
            auto& s1 = X[i - 1];
            auto& s2 = X[i];

            if (s1.size() < s2.size()) {
                // Example: 1
                //          12
                continue;
            }
            
            if (s1.size() == s2.size()) {
                if (s1 < s2) {
                    // Example: 1
                    //          2
                    continue;
                } else {
                    // Example: 2
                    //          1
                    s2 += '0';
                    ++ops;
                    continue;
                }
            }

            const bool prefix = s1.substr(0, s2.size()) == s2;
            if (prefix) {
                // Example: 123456
                //          123
                string suffix;
                bool exceeded = false;

                // Find the right-most digit in s1 that is not a 9.
                for (int j = s1.size() - 1; j >= s2.size(); --j) {
                    if (!exceeded) {
                        if (s1[j] < '9') {
                            replace(suffix.begin(), suffix.end(), '9', '0');
                            suffix += s1[j] + 1;
                            exceeded = true;
                        } else {
                            suffix += '9';
                        }
                    } else {
                        suffix += s1[j];
                    }
                }

                // The suffix is constructed in reverse order.
                reverse(suffix.begin(), suffix.end());

                // The suffix of s1 (after taking away the s2 prefix) is all 9's.
                if (!exceeded) {
                    // Example: 199
                    //          1
                    int padding = s1.size() - s2.size() + 1;
                    suffix = string(padding, '0');
                }

                ops += suffix.size();
                s2 += suffix;
                continue;
            }

            // Append an extra 0 if s2 is lexicographically smaller than s1.
            // Example: 150
            //          13
            const int padding = s1.size() - s2.size() + (s2 < s1);
            s2 += string(padding, '0');
            ops += padding;
        }

        cout << "Case #" << t << ": " << ops << "\n";
    }
}
