#include <bits/stdc++.h>

using namespace std;

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

// -------------------------------------------------------------------------------------

// Graph represents a directed graph.
struct Graph {
    // Adds the given node to this Graph.
    void add_node(int node) {
        nodes.emplace(node);
    }

    // Deletes the given node from this Graph.
    void del_node(int node) {
        for (int cand : nodes) {
            fwd_edges[cand].erase(node);
            bak_edges[cand].erase(node);
        }
        fwd_edges.erase(node);
        bak_edges.erase(node);
        nodes.erase(node);
    }

    // Adds the given edge to this Graph.
    void add_edge(int tail, int head) {
        fwd_edges[tail].emplace(head);
        bak_edges[head].emplace(tail);
    }

    // Deletes the given edge from this Graph.
    void del_edge(int tail, int head) {
        fwd_edges[tail].erase(head);
        bak_edges[head].erase(tail);
    }

    // Reports whether the given node exists in this Graph.
    bool has(int node) {
        return nodes.find(node) != nodes.end();
    }

    // Populates the descendant matrix of this Graph using Floyd–Warshall.
    void connect() {
        int N = size();
        connected = vector<vector<bool>>(N, vector<bool>(N));
        for (int tail = 0; tail < N; ++tail) {
            for (int head : fwd_edges[tail]) {
                connected[tail][head] = true;
            }
        }
        for (int mid = 0; mid < N; ++mid) {
            for (int tail = 0; tail < N; ++tail) {
                for (int head = 0; head < N; ++head) {
                    if (connected[tail][mid] && connected[mid][head]) {
                        connected[tail][head] = true;
                    }
                }
            }
        }
    }

    // Returns the descendants of the given node in this Graph using DFS.
    unordered_set<int> descendants(int node) {
        unordered_set<int> visited;
        queue<int> bfsq;
        bfsq.emplace(node);
        while (!bfsq.empty()) {
            int tail = bfsq.front();
            bfsq.pop();
            for (int head : fwd_edges[tail]) {
                if (visited.find(head) == visited.end()) {
                    visited.emplace(head);
                    bfsq.emplace(head);
                }
            }
        }
        return visited;
    }

    // Reports whether the given node is part of a cycle in this Graph.
    bool cyclic(int node) const {
        return connected[node][node];
    }

    // Reports the maximum size needed to capture all of the nodes in this Graph.
    int size() const {
        return nodes.empty() ? 0 : *max_element(nodes.begin(), nodes.end()) + 1;
    }

    // Returns the bipartite matching produced by Kőnig's theorem.
    Graph match() {
        int N = size();
        int src = 2 * N;
        int dst = src + 1;

        Graph G;
        G.add_node(src);
        G.add_node(dst);
        for (int node : nodes) {
            G.add_node(node);
            G.add_node(N + node);
        }

        Graph M = G;

        for (int tail : nodes) {
            G.add_edge(src, tail);
            G.add_edge(N + tail, dst);
            for (int head : nodes) {
                if (connected[tail][head]) {
                    G.add_edge(tail, N + head);
                }
            }
        }

        while (true) {
            unordered_set<int> visited;
            unordered_set<int> flow_set {src};
            vector<int> flow_vec = {src};

            const function<bool(int)> dfs = [&](int node) {
                if (node == dst) {
                    return true;
                } else if (visited.find(node) != visited.end()) {
                    return false;
                }

                for (int head : G.fwd_edges[node]) {
                    if (flow_set.find(head) == flow_set.end()) {
                        flow_vec.emplace_back(head);
                        flow_set.emplace(head);
                        if (dfs(head)) {
                            return true;
                        }
                        flow_set.erase(head);
                        flow_vec.pop_back();
                    }
                }
                visited.emplace(node);
                return false;
            };

            if (!dfs(src)) {
                break;
            }

            for (int i = 0; i < flow_vec.size() - 1; ++i) {
                int tail = flow_vec[i];
                int head = flow_vec[i + 1];
                G.del_edge(tail, head);
                G.add_edge(head, tail);
            }
            for (int i = 1; i < flow_vec.size() - 2; ++i) {
                int tail = flow_vec[i];
                int head = flow_vec[i + 1];
                if (i % 2 == 1) {
                    M.add_edge(tail, head);
                } else {
                    M.del_edge(head, tail);
                }
            }
        }

        M.del_node(src);
        M.del_node(dst);
        return M;
    }

    unordered_set<int> nodes;
    unordered_map<int, unordered_set<int>> fwd_edges;
    unordered_map<int, unordered_set<int>> bak_edges;
    vector<vector<bool>> connected;
};

// Serialializes a Graph to the given output stream.
ostream& operator<<(ostream& out, const Graph& graph) {
    return out << "(Nodes = " << graph.nodes << ", Fwd = " << graph.fwd_edges << ")";
}

// Solves the task at hand by simulating each possible trace of Khan's algorithm.
int slow(Graph& G) {
    vector<int> sources;
    for (int node : G.nodes) {
        if (G.bak_edges[node].empty()) {
            sources.emplace_back(node);
        }
    }
    int best = sources.size();
    for (int src : sources) {
        Graph residual = G;
        residual.del_node(src);
        best = max(best, slow(residual));
    }
    return best;
}

// Solves the task at hand using Dilworth's theorem.
int fast(int N, Graph& G) {
    // Delete all descendants of a cycle: these can never be reached by Khan's algorithm.
    G.connect();
    for (int node = 0; node < N; ++node) { 
        if (G.has(node) && G.cyclic(node)) {
            for (int desc : G.descendants(node)) {
                G.del_node(desc);
            }
        }
    }

    // Count the number of components in the maximal bipartite matching.
    int size = G.size();
    auto M = G.match();
    int chains = 0;
    for (int node = 0; node < size; ++node) {
        if (M.has(node)) {
            ++chains;
            int root = node;
            while (!M.fwd_edges[root].empty()) {
                root = *M.fwd_edges[root].begin() - size;
            }
            while (!M.bak_edges[size + root].empty()) {
                int prev = *M.bak_edges[size + root].begin();
                M.del_node(root);
                root = prev;
            }
            M.del_node(root);
        }
    }
    return chains;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;

    Graph G;
    for (int node = 0; node < N; ++node) {
        G.add_node(node);
    }
    for (int i = 0; i < M; ++i) {
        int tail, head;
        cin >> tail >> head;
        G.add_edge(tail, head);
    }

    // cout << slow(G) << "\n";
    cout << fast(N, G) << "\n";

    // auto rng = mt19937_64(random_device{}());
    // auto dis_n = uniform_int_distribution<int>(1, 4);
    // auto dis_p = uniform_real_distribution<double>(0, 1);
    // for (int i = 0; i < 10000; ++i) {
    //     int N = dis_n(rng);
    //     int M = 0;
    //     Graph G;
    //     for (int i = 0; i < N; ++i) {
    //         G.add_node(i);
    //     }
    //     for (int i = 0; i < N; ++i) {
    //         for (int j = 0; j < N; ++j) {
    //             if (i != j) {
    //                 if (dis_p(rng) < 0.5) {
    //                     G.add_edge(i, j);
    //                     ++M;
    //                 }
    //             }
    //         }
    //     }
    //     Graph oG = G;
    //     int want = slow(G);
    //     int have = fast(N, G);
    //     if (want != have) {
    //         cerr << "Error: " << oG << "\n";
    //         cerr << "    Want = " << want << "\n";
    //         cerr << "    Have = " << have << "\n";
    //         break;
    //     }
    // }

    return 0;
}