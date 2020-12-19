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

using DP = unordered_map<int, unordered_map<string, bool>>;
using Rule = variant<char, vector2D<int>>;
using Rules = unordered_map<int, Rule>;

bool validate(const Rules& rules, const string& msg, DP& dp, int id, int beg, int end) {
    if (beg == end) {
        return false;
    }

    const auto seg = msg.substr(beg, end - beg);
    if (dp[id].find(seg) != dp[id].end()) {
        return dp[id][seg];
    }

    const auto& rule = rules.at(id);
    if (rule.index() == 0) {
        if (beg == end - 1) {
            char letter = get<0>(rule);
            return dp[id][seg] = letter == msg[beg];
        }
    } else {
        for (const auto& part : get<1>(rule)) {
            if (part.size() == 1) {
                if (validate(rules, msg, dp, part.front(), beg, end)) {
                    return dp[id][seg] = true;
                }
            } else if (part.size() == 2) {
                for (int mid = beg + 1; mid < end; ++mid) {
                    if (validate(rules, msg, dp, part.front(), beg, mid) && validate(rules, msg, dp, part.back(), mid, end)) {
                        return dp[id][seg] = true;
                    }
                }
            } else if (part.size() == 3) {
                for (int i = beg + 1; i < end - 1; ++i) {
                    for (int j = i + 1; j < end; ++j) {
                        if (validate(rules, msg, dp, part[0], beg, i) && validate(rules, msg, dp, part[1], i, j) && validate(rules, msg, dp, part[2], j, end)) {
                            return dp[id][seg] = true;
                        }
                    }
                }
            } else {
                cerr << "Error: unexpected part size " << part.size() << ".\n";
                return dp[id][seg] = false;
            }
        }
    }
    return dp[id][seg] = false;
}

void solution() {
    string line;
    Rules rules;
    while (getline(cin, line) && !line.empty()) {
        const auto tokens = split(line, " ");
        int id = stoi(tokens.front().substr(0, tokens.front().size() - 1));
        if (tokens[1].front() == '\"') {
            rules[id] = tokens.back()[1];
        } else {
            vector2D<int> parts;
            vector<int> part;
            for (int i = 1; i < tokens.size(); ++i) {
                if (tokens[i] == "|") {
                    parts.emplace_back(part);
                    part.clear();
                } else {
                    part.emplace_back(stoi(tokens[i]));
                }
            }
            parts.emplace_back(part);
            rules[id] = parts;
        }
    }

    DP dp;
    int valid = 0;
    while (getline(cin, line) && !line.empty()) {
        valid += validate(rules, line, dp, 0, 0, line.size());
    }
    cout << valid << '\n';
}
