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
    using Ticket = vector<int>;

    Ticket my_ticket;
    getline(cin, line);
    getline(cin, line);
    for (const auto& value : split(line, ",")) {
        my_ticket.emplace_back(stoi(value));
    }

    vector<Ticket> nearby_tickets;
    getline(cin, line);
    getline(cin, line);
    while (getline(cin, line) && !line.empty()) {
        Ticket ticket;
        for (const auto& value : split(line, ",")) {
            ticket.emplace_back(stoi(value));
        }
        bool valid = true;
        for (int i = 0; i < ticket.size() && valid; ++i) {
            valid = false;
            int value = ticket[i];
            for (int j = 0; j < fields.size() && !valid; ++j) {
                valid = fields[j].contains(value);
            }
        }
        // Skip over tickets that have invalid fields.
        if (valid) {
            nearby_tickets.emplace_back(ticket);
        }
    }

    // Find a matching between ticket indices and fields using max flow.
    int num_fields = fields.size();
    int src_node = 2 * num_fields;
    int dst_node = 2 * num_fields + 1;
    vector<unordered_set<int>> graph(2 * num_fields + 2);
    for (int i = 0; i < num_fields; ++i) {
        int lhs_node = i;
        int rhs_node = num_fields + i;
        graph[src_node].emplace(lhs_node);
        graph[rhs_node].emplace(dst_node);
    }

    // Create edges between ticket indices and candidate fields.
    for (int i = 0; i < num_fields; ++i) {
        for (int j = 0; j < num_fields; ++j) {
            bool possible = true;
            for (const auto& ticket : nearby_tickets) {
                possible = possible && fields[j].contains(ticket[i]);
            }
            if (possible) {
                graph[i].emplace(num_fields + j);
            }
        }
    }

    // The residual graph of Edmonds–Karp will contain the desired matching.
    while (true) {
        vector<int> pred(graph.size(), -1);
        pred[src_node] = src_node;
        queue<int> bfsq;
        bfsq.emplace(src_node);
        while (!bfsq.empty() && bfsq.front() != dst_node) {
            int node = bfsq.front();
            bfsq.pop();
            for (int adjacent : graph[node]) {
                if (pred[adjacent] == -1) {
                    pred[adjacent] = node;
                    bfsq.emplace(adjacent);
                }
            }
        }
        if (pred[dst_node] == -1) {
            // No path exists between the source and destination nodes.
            break;
        }
        // Compute the residual graph.
        for (int next = dst_node; pred[next] != next; next = pred[next]) {
            int prev = pred[next];
            graph[prev].erase(next);
            graph[next].emplace(prev);
        }
    }

    long long product = 1;
    for (int i = 0; i < fields.size(); ++i) {
        const auto& field = fields[i];
        if (field.name.substr(0, 9) == "departure") {
            int index = *graph[num_fields + i].begin();
            product *= my_ticket[index];
        }
    }
    cout << product << '\n';
}
