#include <bits/stdc++.h>

using namespace std;

// Macros
#ifdef D3BUG
    #define cerrd if (D3BUG) cerr
#else
    #define cerrd if (false) cerr
#endif

// Types
template<class T> using vector1D = vector<T>;
template<class T> using vector2D = vector<vector1D<T>>;
template<class T> using vector3D = vector<vector2D<T>>;
template<class T> using uset = unordered_set<T>;
template<class K, class V> using umap = unordered_map<K, V>;

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

struct Snake {
    queue<pair<int, int>> Q;
    uset<pair<int, int>> S;
    int dir;
};

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int S, R, C;
        cin >> S >> R >> C;
        vector<pair<int, char>> actions(S);
        for (int i = 0; i < S; ++i) {
            cin >> actions[i].first >> actions[i].second;
        }
        const vector<pair<int, int>> dirs = {{R - 1, 0}, {0, 1}, {1, 0}, {0, C - 1}};
        uset<pair<int, int>> eaten;
        Snake snake;
        snake.Q.emplace(0, 0);
        snake.S.emplace(0, 0);
        snake.dir = 1;
        int time = actions[S - 1].first + max(R, C) + 2;
        bool over = false;
        auto action = actions.begin();
        for (int i = 1; i <= time && !over; ++i) {
            auto head = snake.Q.back();
            head.first += dirs[snake.dir].first;
            head.first %= R;
            head.second += dirs[snake.dir].second;
            head.second %= C;

            bool odd = (head.first + head.second) % 2 == 1;
            if (odd && eaten.find(head) == eaten.end()) {
                eaten.emplace(head);
            } else {
                auto tail = snake.Q.front();
                snake.Q.pop();
                snake.S.erase(tail);
            }

            if (snake.S.find(head) != snake.S.end()) {
                over = true;
            }
            snake.Q.emplace(head);
            snake.S.emplace(head);

            if (action != actions.end() && action->first == i) {
                char left = action->second == 'L';
                if (left) {
                    snake.dir = (snake.dir + 3) % 4;
                } else {
                    snake.dir = (snake.dir + 1) % 4;
                }
                ++action;
            }
        }
        cout << "Case #" << t << ": " << snake.Q.size() << "\n";
    }
}
