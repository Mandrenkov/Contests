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

template<
    // Output stream overload for STL containers with one explicit type parameter.
    template<class> class Range, class T,
    typename = enable_if_t<is_same_v<Range<T>, deque<T>> ||
                           is_same_v<Range<T>, forward_list<T>> ||
                           is_same_v<Range<T>, list<T>> ||
                           is_same_v<Range<T>, multiset<T>> ||
                           is_same_v<Range<T>, set<T>> ||
                           is_same_v<Range<T>, unordered_multiset<T>> ||
                           is_same_v<Range<T>, unordered_set<T>> ||
                           is_same_v<Range<T>, vector<T>>>
>
ostream& operator<<(ostream& out, const Range<T>& range) {
    constexpr auto brackets = is_same_v<Range<T>, vector<T>> ? "[]" : "{}";
    out << brackets[0];
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (next(it) != range.end() ? ", " : "");
    }
    return out << brackets[1];
}

template<
    // Output stream overload for STL containers with two explicit type parameters.
    template<class, class> class Range, class K, class V,
    typename = enable_if_t<is_same_v<Range<K, V>, map<K, V>> ||
                           is_same_v<Range<K, V>, multimap<K, V>> ||
                           is_same_v<Range<K, V>, unordered_map<K, V>> ||
                           is_same_v<Range<K, V>, unordered_multimap<K, V>>>
>
ostream& operator<<(ostream& out, const Range<K, V>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << it->first << ": " << it->second << (next(it) != range.end() ? ", " : "");
    }
    return out << '}';
}

template <class F, class S>
ostream& operator<<(ostream& out, const pair<F, S>& duo) {
  return out << '(' << duo.first << ", " << duo.second << ')';
}

// Hash Template Specializations
template<class T, class U>
struct hash<std::pair<T, U>> {
    size_t operator()(const std::pair<T, U>& pair) const noexcept {
        return std::hash<T>{}(pair.first) ^ (std::hash<U>{}(pair.second) << 1);
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

// Splits |text| using |sep| as the delimeter string; empty tokens are omitted from the returned vector.
vector<string> split(const string& text, const string&& sep) {
    auto prev = text.begin();
    auto next = text.begin();
    vector<string> tokens;
    for (bool first = true; next != text.end(); first = false) {
        prev = first ? text.begin() : next + sep.size();
        next = search(prev, text.end(), sep.begin(), sep.end());
        if (prev != next) {
            tokens.emplace_back(prev, next);
        }
    }
    return tokens;
}

// -----------------------------------------------------------------------------

void solution() {
    vector<pair<vector<string>, vector<string>>> inputs;
    string line;
    while (getline(cin, line) && !line.empty()){
        const auto tokens = split(line, " ");
        const auto divider = distance(tokens.begin(), find(tokens.begin(), tokens.end(), "(contains"));
        pair<vector<string>, vector<string>> input;
        for (int i = 0; i < divider; ++i) {
            input.first.emplace_back(tokens[i]);
        }
        for (int i = divider + 1; i < tokens.size(); ++i) {
            const auto allergen = string(tokens[i].begin(), --tokens[i].end());
            input.second.emplace_back(allergen);
        }
        inputs.emplace_back(input);
    }

    unordered_map<string, int> ingredients;
    unordered_map<string, unordered_set<string>> candidate_map;
    for (const auto& input : inputs) {
        for (const auto& ingredient : input.first) {
            ++ingredients[ingredient];
        }
        for (const auto& allergen : input.second) {
            if (candidate_map.find(allergen) == candidate_map.end()) {
                candidate_map[allergen] = unordered_set<string>(input.first.begin(), input.first.end());
            } else {
                unordered_set<string> intersection(input.first.begin(), input.first.end());
                for (const auto& candidate : input.first) {
                    if (candidate_map[allergen].find(candidate) == candidate_map[allergen].end()) {
                        intersection.erase(candidate);
                    }
                }
                candidate_map[allergen] = intersection;
            }
        }
    }

    int total = 0;
    for (const auto& [ingredient, counter] : ingredients) {
        bool safe = true;
        for (auto it = candidate_map.begin(); it != candidate_map.end() && safe; ++it) {
            safe = it->second.find(ingredient) == it->second.end();
        }
        total += safe * counter;
    }
    cout << total << '\n';
}
