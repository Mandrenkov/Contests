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

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;
        vector<pair<int, int>> squares(N);
        vector<vector<char>> board(8, vector<char>(8, '.'));
        for (int i = 0; i < N; ++i) {
            string line;
            cin >> line;
            int row = line[0] - 'A';
            int col = line[1] - '1';
            board[row][col] = line.back();
            squares[i] = {row, col};
        }
        int attacks = 0;
        for (const auto [dst_row, dst_col] : squares) {
            for (const auto [src_row, src_col] : squares) {
                int attacks_before = attacks;
                if (src_row == dst_row && src_col == dst_col) {
                    continue;
                }
                int dr = dst_row - src_row;
                int dc = dst_col - src_col;
                char piece = board[src_row][src_col];
                if (piece == 'P') {
                    attacks += dr == 1 && abs(dc) == 1;
                } else if (piece == 'R') {
                    if (dr != 0 && dc != 0) {
                        continue;
                    }
                    int dist = max(abs(dr), abs(dc));
                    bool open = true;
                    for (int k = 1; k < dist && open; ++k) {
                        int r = src_row + k * dr / dist;
                        int c = src_col + k * dc / dist;
                        open = board[r][c] == '.';
                    }
                    attacks += open;
                } else if (piece == 'N') {
                    attacks += (abs(dr) == 2 && abs(dc) == 1) || (abs(dc) == 2 && abs(dr) == 1);
                } else if (piece == 'B') {
                    if (abs(dr) != abs(dc)) {
                        continue;
                    }
                    bool open = true;
                    for (int k = 1; k < abs(dr) && open; ++k) {
                        int r = src_row + k * dr / abs(dr);
                        int c = src_col + k * dc / abs(dc);
                        open = board[r][c] == '.';
                    }
                    attacks += open;
                } else if (piece == 'K') {
                    int dist = max(abs(dr), abs(dc));
                    attacks += dist <= 1;
                } else if (piece == 'Q') {
                    if (dr == 0 || dc == 0) {
                        int dist = max(abs(dr), abs(dc));
                        bool open = true;
                        for (int k = 1; k < dist && open; ++k) {
                            int r = src_row + k * dr / dist;
                            int c = src_col + k * dc / dist;
                            open = board[r][c] == '.';
                        }
                        attacks += open;
                    } else if (abs(dr) == abs(dc)) {
                        bool open = true;
                        for (int k = 1; k < abs(dr) && open; ++k) {
                            int r = src_row + k * dr / abs(dr);
                            int c = src_col + k * dc / abs(dc);
                            open = board[r][c] == '.';
                        }
                        attacks += open;
                    }
                }
            }
        }
        cout << "Case #" << t << ": " << attacks << "\n";
    }
}
