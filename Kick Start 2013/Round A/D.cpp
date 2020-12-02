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
    // Two explicit template parameters are needed for containers like std::map.
    template<class, class> class Range, class K, class V,
    // This overload must be disabled for std::string to avoid ambiguity.
    typename = enable_if_t<!is_same_v<Range<K, V>, string>>
>
ostream& operator<<(ostream& out, const Range<K, V>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (next(it) != range.end() ? ", " : "");
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

string simulate(int N, const vector<string>& maze, const pair<int, int>& S, const pair<int, int>& E) {
    if (maze[S.first - 1][S.second] == '#' &&
        maze[S.first + 1][S.second] == '#' &&
        maze[S.first][S.second - 1] == '#' &&
        maze[S.first][S.second + 1] == '#') {
        // There is nowhere to go.
        return "Edison ran out of energy.";
    }
    vector<pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int dir = S == pair<int, int>{1, N} ? 2 : 0;
    int row = S.first;
    int col = S.second;
    const auto turn = [&]() {
        // Turn left and then as many times right as it takes for an opening to be found.
        dir = (dir + 3) % 4;
        for (int i = 0; i < 4; ++i) {
            pair<int, int> next = {row + dirs[dir].first, col + dirs[dir].second};
            if (maze[next.first][next.second] == '.') {
                return;
            } else {
                dir = (dir + 1) % 4;
            }
        }
    };
    string steps;
    for (int i = 0; i < 10000 && pair<int, int>{row, col} != E; ++i) {
        turn();
        steps += "NESW"[dir];
        row += dirs[dir].first;
        col += dirs[dir].second;
    }
    if (pair<int, int>{row, col} != E) {
        return "Edison ran out of energy.";
    } else {
        return to_string(steps.size()) + "\n" + steps;
    }
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;
        vector<string> maze(N + 2);
        maze.front() = maze.back() = string(N + 2, '#');
        for (int i = 1; i <= N; ++i) {
            cin >> maze[i];
            maze[i] = string(1, '#') + maze[i] + string(1, '#');
        }
        // cerr << maze << '\n';
        pair<int, int> S, E;
        cin >> S.first >> S.second >> E.first >> E.second;
        string ans = simulate(N, maze, S, E);
        cout << "Case #" << t << ": " << ans << '\n';
    }
}