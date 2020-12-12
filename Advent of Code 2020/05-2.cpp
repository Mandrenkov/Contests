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
std::ostream& operator<<(std::ostream& out, const std::pair<F, S>& pair) {
    return out << '(' << pair.first << ", " << pair.second << ')';
}

// Hash Template Specializations
template<class T, class U>
struct hash<std::pair<T, U>> {
    size_t operator()(const std::pair<T, U>& pair) const noexcept {
        return std::hash<T>{}(pair.first) ^ (std::hash<U>{}(pair.second) << 1);
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

// Returns the ID of the given boarding pass.
int identify(const string& pass) {
    int id = 0;
    for (int i = 0; i < 10; ++i) {
        id = (id << 1) | (pass[i] == 'B' || pass[i] == 'R');
    }
    return id;
}

void solution() {
    int N;
    cin >> N;
    vector<string> passes(N);
    for (int i = 0; i < N; ++i) {
        cin >> passes[i];
    }
    set<int> ids;
    for (int i = 0; i < N; ++i) {
        int id = identify(passes[i]);
        ids.emplace(id);
    }
    set<int> seats;
    int lo = *ids.begin();
    int hi = *ids.rbegin();
    for (int i = lo; i <= hi; ++i) {
        seats.emplace(i);
    }
    for (const int id : ids) {
        seats.erase(id);
    }
    cout << *seats.begin() << '\n';
}
