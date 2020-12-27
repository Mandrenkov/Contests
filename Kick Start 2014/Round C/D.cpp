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

vector3D<bool> tetrominos = {
    {
        {1, 0},
        {1, 1},
        {0, 1}
    }, {
        {0, 1},
        {1, 1},
        {1, 0},
    }, {
        {1, 0},
        {1, 0},
        {1, 1},
    }, {
        {0, 1},
        {0, 1},
        {1, 1},
    }, {
        {1, 1},
        {1, 1},
    }, {
        {1},
        {1},
        {1},
        {1}
    }, {
        {0, 1, 0},
        {1, 1, 1},
    }
};

vector2D<bool> rotateCCW(vector2D<bool> tetromino, int turns) {
    for (int turn = 0; turn < 4 - turns; ++turn) {
        int rows = tetromino.size();
        int cols = tetromino[0].size();
        vector2D<bool> rotated_tetromino(cols, vector<bool>(rows));
        for (int r = 0; r < cols; ++r) {
            for (int c = 0; c < rows; ++c) {
                // [0  1] * [x] = [ y]
                // [-1 0]   [y]   [-x]
                rotated_tetromino[r][c] = tetromino[-(c - rows) - 1][r];
            }
        }
        tetromino = rotated_tetromino;
    }
    return tetromino;
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int W, H, N;
        cin >> W >> H >> N;
        vector2D<bool> prev_grid(H + 5, vector<bool>(W, false));
        prev_grid.back() = vector<bool>(W, true);
        bool over = false;
        for (int i = 0; i < N; ++i) {
            int type, turns, x;
            cin >> type >> turns >> x;
            if (over) {
                continue;
            }
            const auto tetronimo = tetrominos[type - 1];
            const auto rotated_tetronomino = rotateCCW(tetronimo, turns);
            int h = rotated_tetronomino.size();
            int w = rotated_tetronomino[0].size();
            int hit = -1;
            for (int y = 1; y < prev_grid.size() && hit == -1; ++y) {
                for (int r = 0; r < h && hit == -1; ++r) {
                    for (int c = 0; c < w && hit == -1; ++c) {
                        if (rotated_tetronomino[r][c] && prev_grid[y + r][x + c]) {
                            hit = y - 1;
                        }
                    }
                }
            }
            for (int r = 0; r < h; ++r) {
                for (int c = 0; c < w; ++c) {
                    if (rotated_tetronomino[r][c]) {
                        prev_grid[hit + r][x + c] = true;
                    }
                }
            }
            for (int y = 0; y < 4 && !over; ++y) {
                over = any_of(prev_grid[y].begin(), prev_grid[y].end(), [](const bool occupied) {return occupied;});
            }
            vector2D<bool> next_grid = {prev_grid.back()};
            for (int y = prev_grid.size() - 2; y >= 0; --y) {
                bool filled = all_of(prev_grid[y].begin(), prev_grid[y].end(), [](const bool occupied) {return occupied;});
                if (!filled) {
                    next_grid.emplace_back(prev_grid[y]);
                }
            }
            int padding = prev_grid.size() - next_grid.size();
            for (int j = 0; j < padding; ++j) {
                next_grid.emplace_back(W, false);
            }
            reverse(next_grid.begin(), next_grid.end());
            prev_grid = next_grid;
        }
        cout << "Case #" << t << ":\n";
        if (over) {
            cout << "Game Over!\n";
        } else {
            for (int y = 4; y < prev_grid.size() - 1; ++y) {
                for (int x = 0; x < W; ++x) {
                    cout << (prev_grid[y][x] ? 'x' : '.');
                }
                cout << '\n';
            }
        }
    }
}
