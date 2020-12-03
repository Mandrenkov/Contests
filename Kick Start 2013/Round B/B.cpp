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
    string text;
    string line;
    while (getline(cin, line)) {
        text += line;
        text += '\n';
    }
    int state = 0;
    int level = 0;
    string program;
    for (int i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (level == 0) {
            program += c;
        }
        switch (state) {
            case 0:
                if (c == '/') {
                    state = 1;
                } else if (level > 0 && c == '*') {
                    state = 2;
                }
                break;
            case 1:
                if (c == '/') {
                    // Do Nothing
                } else if (c == '*') {
                    if (level == 0) {
                        program.pop_back();
                        program.pop_back();
                    }
                    ++level;
                    state = 0;
                } else {
                    state = 0;
                }
                break;
            case 2:
                if (c == '*') {
                    // Do Nothing
                } else if (c == '/') {
                    --level;
                    state = 0;
                } else {
                    state = 0;
                }
                break;
        }
    }
    cout << "Case #1:\n" << program << '\n';
}
