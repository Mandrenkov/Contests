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

template <typename T>
struct SegmentTree {
    int N;
    std::vector<T> t;
    std::function<T(T, T)> combine;
    T zero;

    static T combine_sum(T a, T b) { return a + b; };
    static T combine_min(T a, T b) { return std::min(a, b); };
    static T combine_max(T a, T b) { return std::max(a, b); };

    SegmentTree(const int size = 1, function<T(T, T)> combine_func = combine_sum, T zero = T{}) : N(size), combine(combine_func), zero(zero) {
        t = std::vector<T>(2 * N, zero);
    }

    SegmentTree(const vector<T> array, function<T(T, T)> combine_func = combine_sum, T zero = T{}) : N(array.size()), combine(combine_func), zero(zero) {
        t = std::vector<T>(2 * N, zero);
        for (int i = N; i < 2 * N; i++) t[i] = array[i - N];
        build();
    }

    /**
     * Build the tree - O(N).
     */
    void build() {
        for (int i = N - 1; i > 0; --i)
          t[i] = combine(t[i << 1], t[i << 1 | 1]);
    }

    /**
     * Modification on single element, interval access.
     * Set position in the tree to the given value, adjust tree - O(log(N)).
     */
    void modify(int p, const T value) {
        for (t[p += N] = value; p > 1; p >>= 1)
          t[p >> 1] = combine(t[p], t[p ^ 1]);
    }

    /**
     * Modification on interval, single element access.
     * Set interval in the tree to given value, adjust tree - O(log(N)).
     */
    void modify(int l, int r, const T value) {
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l & 1) {
                t[l] += value;
                l++;
            }
            if (r & 1) {
                r--;
                t[r] += value;
            }
        }
    }

    /**
     * Modification on single element, interval access.
     * Get result on interval [l, r) - O(log(N)).
     */
    T query(int l, int r) {
        T resl = zero, resr = zero;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = combine(resl, t[l++]);
            if (r & 1) resr = combine(t[--r], resr);
        }
        return combine(resl, resr);
    }

    /**
     * Modification on interval, single element access.
     * Get result for single element - O(log(N)).
     */
    T query(int p) {
        T res = zero;
        for (p += N; p > 0; p >>= 1) res = combine(res, t[p]);
        return res;
    }

    /**
     * Modification on interval, single element access.
     * Force all values to leaf nodes - O(N).
     */
    void push() {
        for (int i = 1; i < N; ++i) {
            t[i << 1] = combine(t[i << 1], t[i]);
            t[i << 1 | 1] = combine(t[i << 1 | 1], t[i]);
            t[i] = zero;
        }
    }

    T operator[](int i) {
        return t[i + N];
    }

    T set(int i, T val) {
        return t[i + N] = val;
    }
};

template <typename T>
ostream& operator<<(ostream& out, const SegmentTree<T>& st) {
    for (int i = 0; i < 2 * st.N; i++) out << st.t[i] << " ";
    return out;
}

// -----------------------------------------------------------------------------

struct City {
    int id;
    int road_id;
    int parent_id;
    unordered_map<int, int> city_id_to_road_id_map;
};

struct Road {
    int id;
    int city_id_1;
    int city_id_2;
    int limit;
    unsigned long long charge;
};

struct Query {
    int id;
    int city_id;
    int load;
};

// Using a structure rather than global data helps reset the variables between cases.
struct Data {
    vector<City> cities;
    vector<Road> roads;
    vector<Query> queries;

    unordered_map<int, vector<int>> city_id_to_query_ids_map;
    vector<unsigned long long> output;
    SegmentTree<unsigned long long> segtree;
};

// Orients the tree by recursively setting the parent ID of each city.
void orient(Data& data, int prev_city_id, int curr_city_id) {
    auto& city = data.cities[curr_city_id];

    city.road_id = city.city_id_to_road_id_map[prev_city_id];
    city.parent_id = prev_city_id;

    city.city_id_to_road_id_map.erase(prev_city_id);
    for (const auto [next_city_id, _] : city.city_id_to_road_id_map) {
        orient(data, curr_city_id, next_city_id);
    }
}

// Populates the output array by traversing the cities using DFS, pushing or popping
// the limit and charge of each road in the segtree as it is traversed.
void answer(Data& data, int city_id) {
    const auto& city = data.cities[city_id];
    const auto& road = data.roads[city.road_id];

    // Account for multiple roads with the same load.
    const bool first = data.segtree[road.limit] == 0;
    data.segtree.modify(road.limit, road.charge);

    for (int query_id : data.city_id_to_query_ids_map[city_id]) {
        auto& query = data.queries[query_id];
        
        // Take the GCD of the road charges with a limit less than or equal to the load.
        unsigned long long min_limit = 0;
        unsigned long long max_limit = min(data.segtree.N, query.load + 1);
        data.output[query_id] = data.segtree.query(min_limit, max_limit);
    }

    for (const auto& [next_city_id, _] : city.city_id_to_road_id_map) {
        answer(data, next_city_id);
    }

    // Only erase the charge if it was added by this road.
    if (first) {
        data.segtree.modify(road.limit, 0);
    }
}

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, Q;
        cin >> N >> Q;

        Data data;
        auto& cities = data.cities;
        auto& roads = data.roads;
        auto& queries = data.queries;

        cities.resize(N + 1);
        for (int i = 1; i <= N; ++i) {
            cities[i].id = i;
        }

        roads.resize(N - 1);
        for (int i = 0; i < N - 1; ++i) {
            roads[i].id = i;
        }

        queries.resize(Q);
        for (int i = 0; i < Q; ++i) {
            queries[i].id = i;
        }

        data.output.resize(Q);

        for (int i = 0; i < N - 1; ++i) {
            auto& road = roads[i];
            cin >> road.city_id_1 >> road.city_id_2 >> road.limit >> road.charge;
            cities[road.city_id_1].city_id_to_road_id_map[road.city_id_2] = road.id;
            cities[road.city_id_2].city_id_to_road_id_map[road.city_id_1] = road.id;
        }

        for (int i = 0; i < Q; ++i) {
            auto& query = queries[i];
            cin >> query.city_id >> query.load;
            data.city_id_to_query_ids_map[query.city_id].emplace_back(query.id);
        }

        // The maximum road limits determines the size of the segtree.
        int max_road_limit = 0;
        for (const auto& road : roads) {
            max_road_limit = max(max_road_limit, road.limit);
        }

        // Returns the GCD of the LHS and RHS, unless one of them is zero, in which
        // case the other number is returned.
        const auto combine = [](unsigned long long lhs, unsigned long long rhs) {
            return gcd(lhs, rhs);
        };
        data.segtree = SegmentTree<unsigned long long>(max_road_limit + 1, combine);

        // Do not "visit" the root city since it has no roads to contribute.
        for (const auto& [city_id, _] : cities[1].city_id_to_road_id_map) {
            orient(data, 1, city_id);
            answer(data, city_id);
        }

        cout << "Case #" << t << ":";
        for (int i = 0; i < Q; ++i) {
            cout << ' ' << data.output[i];
        }
        cout << '\n';
    }
}
