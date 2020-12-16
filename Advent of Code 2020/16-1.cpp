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

struct Field {
    string name;
    pair<int, int> range_1;
    pair<int, int> range_2;

    Field(const string& line) {
        const auto parse_range = [&](const string& text) {
            const auto tokens = split(text, "-");
            return pair<int, int>{stoi(tokens.front()), stoi(tokens.back())};
        };

        const auto colon_tokens = split(line, ":");
        const auto space_tokens = split(colon_tokens.back(), " ");
        name = colon_tokens.front();
        range_1 = parse_range(space_tokens[0]);
        range_2 = parse_range(space_tokens[2]);
    }

    bool contains(int value) const {
        return (range_1.first <= value && value <= range_1.second) ||
               (range_2.first <= value && value <= range_2.second);
    }
};

void solution() {
    vector<Field> fields;
    string line;
    while (getline(cin, line) && !line.empty()) {
        Field field(line);
        fields.emplace_back(field);
    }
    for (int i = 0; i < 4; ++i) {
        // Skip lines until the "nearby tickets" section starts.
        getline(cin, line);
    }
    int rate = 0;
    while (getline(cin, line) && !line.empty()) {
        const auto ticket = split(line, ",");
        for (int i = 0; i < ticket.size(); ++i) {
            int value = stoi(ticket[i]);
            bool valid = false;
            for (int j = 0; j < fields.size() && !valid; ++j) {
                valid = fields[j].contains(value);
            }
            if (!valid) {
                rate += value;
            }
        }
    }
    cout << rate << '\n';
}
