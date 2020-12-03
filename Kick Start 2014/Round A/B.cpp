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
    const vector<int> ideals = {
        0b1111110,
        0b0110000,
        0b1101101,
        0b1111001,
        0b0110011,
        0b1011011,
        0b1011111,
        0b1110000,
        0b1111111,
        0b1111011
    };

    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;
        vector<int> displays(N);
        for (int i = 0; i < N; ++i) {
            string display;
            cin >> display;
            for (int j = 0; j < 7; ++j) {
                displays[i] |= (display[j] == '1') << (6 - j);
            }
        }
        unordered_set<int> possible;
        for (int mask = 0; mask < (1 << 7) && possible.size() < 2; ++mask) {
            for (int i = 0; i < 10; ++i) {
                bool ok = true;
                for (int j = 0; j < min(N, 10) && ok; ++j) {
                    int k = (i - j + 10) % 10;
                    if ((mask & ideals[k]) != displays[j]) {;
                        ok = false;
                    }
                }
                if (ok) {
                    int k = (10 + (i - N) % 10) % 10;
                    int display = mask & ideals[k];
                    possible.emplace(display);
                }
            }
        }
        if (possible.size() != 1) {
            cout << "Case #" << t << ": ERROR!\n";
        } else {
            cout << "Case #" << t << ": " << bitset<7>(*possible.begin()) << '\n';
        }
    }
}
