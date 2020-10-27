#include <bits/stdc++.h>

using namespace std;

struct UnionFind {
    /**
     * The parent site of each site.
     */
    int* id;

    /**
     * The size of the component rooted at each site.
     */
    int* sz;

    /**
     * The total number of sites.
     */
    int sites;

    /**
     * The number of active components.
     */
    int comps;

    /**
     * Constructs a union-find instance with the given capacity.
     *
     * @param capacity The initial number of contiguous components.
     */
    UnionFind(const int capacity) : sites(capacity), comps(capacity) {
        id = new int[capacity];
        sz = new int[capacity];
        std::iota(id, id + capacity, 0);
        std::fill(sz, sz + capacity, 1);
    }

    /**
     * Destructs the UnionFind instance.
     */
    ~UnionFind() {
        delete[] id;
        delete[] sz;
    }

    /**
     * Connects two sites.  The ordering of these sites does not matter.
     *
     * @param site1 The first site.
     * @param site2 The second site.
     */
    void merge(const int site1, const int site2) {
        int root1 = find(site1);
        int root2 = find(site2);
        if (root1 == root2) return;

        if (sz[root1] < sz[root2]) {
            id[root1] = root2;
            sz[root2] += sz[root1];
        } else {
            id[root2] = root1;
            sz[root1] += sz[root2];
        }
        --comps;
    }

    /**
     * Returns the component identifier for the given site.
     *
     * @param site The site.
     *
     * @return The component identifier of the site.
     */
    int find(const int site) {
        int root = site == id[site] ? site : find(id[site]);
        id[site] = root;
        return root;
    }

    /**
     * Returns the component identifier for the given site.
     *
     * @param site The site.
     *
     * @return The component identifier of the site.
     */
    int find(const int site) const {
        return site == id[site] ? site : find(id[site]);
    }

    /**
     * Returns true if the two sites belong to the same component.
     *
     * @param site1 The first site.
     * @param site2 The second site.
     *
     * @return True if the two sites belong to the same component.
     */
    bool connected(const int site1, const int site2) {
        return find(site1) == find(site2);
    }

    /**
     * Returns true if the two sites belong to the same component.
     *
     * @param site1 The first site.
     * @param site2 The second site.
     *
     * @return True if the two sites belong to the same component.
     */
    bool connected(const int site1, const int site2) const {
        return find(site1) == find(site2);
    }
};

// Extracts the weakly-connected components from the given graph.
vector<vector<int>> extractComponents(const vector<vector<int>>& G) {
    const int N = G.size();
    UnionFind uf(N);
    for (int i = 0; i < N; ++i) {
        for (int j : G[i]) {
            uf.merge(i, j);
        }
    }

    map<int, vector<int>> component_map;
    for (int i = 0; i < N; ++i) {
        int component_idx = uf.find(i);
        component_map[component_idx].emplace_back(i);
    }

    vector<vector<int>> component_vec;
    component_vec.reserve(uf.comps);
    for (const auto& pair : component_map) {
        component_vec.emplace_back(pair.second);
    }
    return component_vec;
}

// Extracts the strongly-connected components from the given weakly-connected component.
pair<vector<int>, vector<vector<int>>> extractCycles(const vector<vector<int>>& G, const vector<vector<int>>& T, const vector<int>& component, const vector<int>& indices) {
    vector<bool> visited(component.size(), false);
    vector<int> order;
    function<void(int)> visit = [&](int index) {
        if (visited[index]) {
            return;
        }
        visited[index] = true;
        for (int j : G[component[index]]) {
            int index = indices[j];
            visit(index);
        }
        order.push_back(index);
    };
    for (int i = 0; i < component.size(); ++i) {
        visit(i);
    }

    vector<int> assignment(component.size(), -1);
    for (int i = 0; i < component.size(); ++i) {
        int index = order[component.size() - 1 - i];
        if (assignment[index] == -1) {
            vector<int> stack = {index};
            while (!stack.empty()) {
                int top = stack.back();
                stack.pop_back();
                if (assignment[top] == -1) {
                    assignment[top] = index;
                    for (int vertex : T[component[top]]) {
                        stack.push_back(indices[vertex]);
                    }
                }
            }
        }
    }

    vector<vector<int>> cycles(component.size());
    for (int i = 0; i < component.size(); ++i) {
        int cycle = assignment[i];
        cycles[cycle].emplace_back(i);
    }
    return {assignment, cycles};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> G(N);
    vector<vector<int>> T(N);
    for (int i = 0; i < M; ++i) {
        int X, Y;
        cin >> X >> Y;
        G[X].emplace_back(Y);
        T[Y].emplace_back(X);
    }

    long long roads = 0;
    vector<int> indices(N);
    const auto components = extractComponents(G);
    for (const auto& component : components) {
        for (int i = 0; i < component.size(); ++i) {
            indices[component[i]] = i;
        }
        const auto [assignment, cycles] = extractCycles(G, T, component, indices);
        for (int i = 0; i < component.size(); ++i) {
            int cycle = assignment[i];
            roads += component.size() - cycles[cycle].size();
            for (int vertex : G[component[i]]) {
                if (cycle != assignment[indices[vertex]]) {
                    --roads;
                }
            }
            for (int vertex : T[component[i]]) {
                if (cycle != assignment[indices[vertex]]) {
                    --roads;
                }
            }
        }
    }
    for (const auto& component : components) {
        roads += component.size() * (N - component.size());
    }
    roads /= 2;

    cout << roads << '\n';

    return 0;
}