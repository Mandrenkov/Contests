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
std::ostream& operator<<(ostream& out, const std::pair<F, S>& pair) {
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

struct Line {
    int num_stations;
    int wait_time;
    vector<int> travel_times;
};

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;
        vector<Line> lines;
        for (int i = 0; i < N; ++i) {
            Line line;
            cin >> line.num_stations >> line.wait_time;
            line.travel_times.resize(line.num_stations - 1);
            for (int i = 0; i < line.num_stations - 1; ++i) {
                cin >> line.travel_times[i];
            }
            lines.emplace_back(line);
        }
        int M;
        cin >> M;
        vector<vector<int>> tunnels(M, vector<int>(5));
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < 5; ++j) {
                cin >> tunnels[i][j];
            }
            for (int j = 0; j < 4; ++j) {
                --tunnels[i][j];
            }
        }

        using Node = pair<int, int>;
        unordered_map<Node, vector<tuple<Node, int, bool>>> G;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < lines[i].num_stations - 1; ++j) {
                Node tail = {i, j};
                Node head = {i, j + 1};
                int time = lines[i].travel_times[j];
                G[tail].emplace_back(head, time, false);
                G[head].emplace_back(tail, time, false);
            }
        }
        for (int i = 0; i < M; ++i) {
            Node tail = {tunnels[i][0],tunnels[i][1]};
            Node head = {tunnels[i][2],tunnels[i][3]};
            int time = tunnels[i][4];
            G[tail].emplace_back(head, time, true);
            G[head].emplace_back(tail, time, true);
        }

        cout << "Case #" << t << ":\n";

        int Q;
        cin >> Q;
        for (int q = 0; q < Q; ++q) {
            Node tail, head;
            cin >> tail.first >> tail.second >> head.first >> head.second;
            --tail.first;
            --tail.second;
            --head.first;
            --head.second;

            unordered_map<Node, unordered_map<bool, int>> times;
            priority_queue<tuple<int, Node, bool>> pq;
            pq.emplace(0, tail, true);
            while (!pq.empty() && times.find(head) == times.end()) {
                auto [time, node, from_tunnel] = pq.top();
                pq.pop();
                if (times[node].find(from_tunnel) != times[node].end()) {
                    continue;
                }
                times[node].emplace(from_tunnel, -time);
                for (const auto [kid, delta, to_tunnel] : G[node]) {
                    if (from_tunnel && !to_tunnel) {
                        pq.emplace(time - delta - lines[node.first].wait_time, kid, to_tunnel);
                    } else {
                        pq.emplace(time - delta, kid, to_tunnel);
                    }
                }
            }
            int best = -1;
            if (times.find(head) != times.end()) {
                best = numeric_limits<int>::max();
                if (times[head].find(false) != times[head].end()) {
                    best = times[head][false];
                }
                if (times[head].find(true) != times[head].end()) {
                    best = min(best, times[head][true]);
                }
            }
            cout << best << '\n';
        }
    }
}
