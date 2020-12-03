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
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int B;
        cin >> B;
        vector<tuple<int, int, int, int>> A(B);
        for (int i = 0; i < B; ++i) {
            cin >> get<0>(A[i]) >> get<1>(A[i]) >> get<2>(A[i]) >> get<3>(A[i]);
        }
        map<long long, long long> counter_x, counter_y;
        for (const auto [x1, y1, x2, y2] : A) {
            for (int y = y1; y <= y2; ++y) {
                for (int x = x1; x <= x2; ++x) {
                    ++counter_x[x];
                    ++counter_y[y];
                }
            }
        }
        const auto get_distances = [](map<long long, long long>& counter) {
            long long dummy = counter.begin()->first - 1;
            counter.emplace(dummy, 0);

            long long prev_cnt = 0;
            long long prev_sum = 0;
            long long next_cnt = 0;
            long long next_sum = 0;
            for (const auto [curr, count] : counter) {
                next_cnt += count;
                next_sum += (curr - dummy) * count;
            }
            map<int, long long> dists;
            long long before = dummy;
            for (const auto [curr, _] : counter) {
                if (curr == dummy) {
                    continue;
                }
                prev_cnt += counter[before];
                prev_sum += prev_cnt * (curr - before);
                next_sum -= next_cnt * (curr - before);
                next_cnt -= counter[curr];
                dists[curr] = prev_sum + next_sum;
                before = curr;
            }
            return dists;
        };
        auto dists_x = get_distances(counter_x);
        auto dists_y = get_distances(counter_y);
        long long best_dist = numeric_limits<long long>::max();
        pair<int, int> best_coords;
        for (const auto [x1, y1, x2, y2] : A) {
            for (int y = y1; y <= y2; ++y) {
                for (int x = x1; x <= x2; ++x) {
                    long long dist = dists_x[x] + dists_y[y];
                    if (dist < best_dist) {
                        best_dist = dist;
                        best_coords = {x, y};
                    } else if (dist == best_dist) {
                        if (x < best_coords.first) {
                            best_dist = dist;
                            best_coords = {x, y};
                        } else if (x == best_coords.first) {
                            if (y < best_coords.second) {
                                best_dist = dist;
                                best_coords = {x, y};
                            }
                        }
                    }
                }
            }
        }
        cout << "Case #" << t << ": " << best_coords.first << ' ' << best_coords.second << ' ' << best_dist << '\n';
    }
}
