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

template <class... Ts>
ostream& operator<<(ostream& out, const tuple<Ts...>& tuple) {
    return out << "(" << get<0>(tuple) << ", " << get<1>(tuple) << ", " << get<2>(tuple) << ")";
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

struct Edge {
    tuple<int, int, int> tail;
    tuple<int, int, int> head;
    int capacity;

    Edge(const tuple<int, int, int>&& tail, const tuple<int, int, int>&& head, const int capacity) : tail(tail), head(head), capacity(capacity) {}
};

ostream& operator<<(ostream& out, const Edge& edge) {
    return out << "{" << "(" << get<0>(edge.tail) << ", " << get<1>(edge.tail) << ", " << get<2>(edge.tail) << ") --> " << "(" << get<0>(edge.head) << ", " << get<1>(edge.head) << ", " << get<2>(edge.head) << ") [" << edge.capacity << "]}";
}

// Max Flow = Min Cut
int compute_flows(int N, const vector2D<char>& board, const vector3D<pair<int, int>>& C, const bool blue) {
    char letter = blue ? 'B' : 'R';
    vector<vector3D<Edge>> G(2, vector3D<Edge>(N + 2, vector2D<Edge>(N + 2)));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (const auto [r, c] : C[i][j]) {
                int capacity = board[i][j] == letter && board[r][c] == letter ? 1 : 0;
                G[1][i][j].emplace_back(tuple<int, int, int>{1, i, j}, tuple<int, int, int>{0, r, c}, capacity);
            }
        }
    }
    for (int i = 0; i < N + 1; ++i) {
        for (int j = 0; j < N; ++j) {
            G[0][i][j].emplace_back(tuple<int, int, int>{0, i, j}, tuple<int, int, int>{1, i, j}, 1);
        }
    }

    if (blue) {
        for (int i = 0; i < N; ++i) {
            G[1][i][N - 1].emplace_back(tuple<int, int, int>{1, i, N - 1}, tuple<int, int, int>{0, N, N}, 1);
            G[1][N][N].emplace_back(tuple<int, int, int>{1, N, N}, tuple<int, int, int>{0, i, N - 1}, 0);

            int capacity = board[i][0] == letter ? 1 : 0;
            G[1][N + 1][N + 1].emplace_back(tuple<int, int, int>{1, N + 1, N + 1}, tuple<int, int, int>{0, i, 0}, capacity);
            G[1][i][0].emplace_back(tuple<int, int, int>{1, i, 0}, tuple<int, int, int>{0, N + 1, N + 1}, 0);
        }
    } else {
        for (int i = 0; i < N; ++i) {
            G[1][N - 1][i].emplace_back(tuple<int, int, int>{1, N - 1, i}, tuple<int, int, int>{0, N, N}, 1);
            G[1][N][N].emplace_back(tuple<int, int, int>{1, N, N}, tuple<int, int, int>{0, N - 1, i}, 0);

            int capacity = board[0][i] == letter ? 1 : 0;
            G[1][N + 1][N + 1].emplace_back(tuple<int, int, int>{1, N + 1, N + 1}, tuple<int, int, int>{0, 0, i}, capacity);
            G[1][0][i].emplace_back(tuple<int, int, int>{1, 0, i}, tuple<int, int, int>{0, N + 1, N + 1}, 0);
        }
    }

    for (int flows = 0; flows < 2; ++flows) {
        vector3D<tuple<int, int, int>> visited(2, vector2D<tuple<int, int, int>>(N + 2, vector<tuple<int, int, int>>(N + 2, {-1, -1, -1})));
        queue<Edge> q;
        for (const auto& edge : G[1][N + 1][N + 1]) {
            if (edge.capacity > 0) {
                q.emplace(edge);
                visited[0][get<1>(edge.head)][get<2>(edge.head)] = edge.tail;
            }
        }

        while (!q.empty() && get<0>(visited[0][N][N]) == -1) {
            const auto edge = q.front();
            // cerr << "Q --> " << edge << '\n';
            q.pop();
            for (const auto& adj : G[get<0>(edge.head)][get<1>(edge.head)][get<2>(edge.head)]) {
                if (adj.capacity > 0 && get<0>(visited[get<0>(adj.head)][get<1>(adj.head)][get<2>(adj.head)]) == -1) {
                    q.emplace(adj);
                    visited[get<0>(adj.head)][get<1>(adj.head)][get<2>(adj.head)] = adj.tail;
                }
            }
        }

        if (get<0>(visited[0][N][N]) == -1) {
            return flows;
        }

        tuple<int, int, int> next = {0, N, N};
        while (next != tuple<int, int, int>{1, N + 1, N + 1}) {
            auto prev = visited[get<0>(next)][get<1>(next)][get<2>(next)];
            for (auto& edge : G[get<0>(next)][get<1>(next)][get<2>(next)]) {
                if (edge.head == prev) {
                    edge.capacity += 1;
                    break;
                }
            }
            for (auto& edge : G[get<0>(prev)][get<1>(prev)][get<2>(prev)]) {
                if (edge.head == next) {
                    edge.capacity -= 1;
                    break;
                }
            }
            next = prev;
        }
    }
    return 2;
}

string solve(const int N, const vector2D<char>& board) {
    map<char, int> counter;
    for (const auto& row : board) {
        for (const char c : row) {
            ++counter[c];
        }
    }
    if (abs(counter['R'] - counter['B']) > 1) {
        return "Impossible";
    }
    vector3D<pair<int, int>> C(N, vector2D<pair<int, int>>(N));
    vector<pair<int, int>> moves = {{-1, 0}, {0, -1}, {-1,  1},
                                    { 1, 0}, {0,  1}, { 1, -1}};
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (const auto [dr, dc] : moves) {
                int row = i + dr;
                int col = j + dc;
                if (0 <= row && row < N && 0 <= col && col < N) {
                    C[i][j].emplace_back(row, col);
                }
            }
        }
    }
    int flows_B = compute_flows(N, board, C, true);
    int flows_R = compute_flows(N, board, C, false);
    int flows = flows_B + flows_R;
    if (flows == 0) {
        return "Nobody wins";
    } else if (flows > 1) {
        return "Impossible";
    }

    if (flows_B == 1 && counter['R'] > counter['B']) {
        return "Impossible";
    } else if (flows_R == 1 && counter['B'] > counter['R']) {
        return "Impossible";
    }

    if (flows_B == 1) {
        return "Blue wins";
    } else {
        return "Red wins";
    }
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;
        vector2D<char> board(N, vector<char>(N));
        for (int i = 0; i < N; ++i) {
            string line;
            cin >> line;
            for (int j = 0; j < N; ++j) {
                board[i][j] = line[j];
            }
        }
        auto ans = solve(N, board);
        cout << "Case #" << t << ": " << ans << '\n';
    }
}
