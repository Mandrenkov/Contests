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

struct Athlete {
    string name;
    vector<int> scores;
    int points;

    Athlete(const string& name) : name(name), points(0) {}
    Athlete(const string& name, int points) : name(name), points(points) {}

    void update(int M) {
        sort(scores.rbegin(), scores.rend());
        points = 0;
        for (int i = 0; i < M && i < scores.size(); ++i) {
            points += scores[i];
        }
    }

    bool operator<(const Athlete& athlete) const {
        return points < athlete.points || (points == athlete.points && name > athlete.name);
    }
};

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int P;
        cin >> P;
        vector<int> S(P);
        for (int i = 0; i < P; ++i) {
            cin >> S[i];
        }
        int N;
        cin >> N;
        unordered_map<string, Athlete> athletes;
        for (int i = 0; i < N; ++i) {
            int weight;
            cin >> weight;
            for (int j = 0; j < P; ++j) {
                string name;
                cin >> name;
                athletes.emplace(name, Athlete(name));
                athletes.at(name).scores.emplace_back(weight * S[j]);
            }
        }
        int M;
        cin >> M;
        vector<Athlete> sorted;
        for (auto& [name, athlete] : athletes) {
            athlete.update(M);
            sorted.emplace_back(athlete.name, athlete.points);
        }
        sort(sorted.rbegin(), sorted.rend());
        cout << "Case #" << t << ":\n";
        pair<int, int> last = {numeric_limits<int>::max(), -1};
        for (int i = 0; i < sorted.size(); ++i) {
            if (sorted[i].points < last.first) {
                last = {sorted[i].points, i + 1};
            }
            cout << last.second << ": " << sorted[i].name << "\n";
        }
    }
}
