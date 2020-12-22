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

string serialize(deque<int>* cards) {
    string code;
    for (const int card : cards[0]) {
        code += to_string(card);
        code += ',';
    }
    code.pop_back();
    code += '|';
    for (const int card : cards[1]) {
        code += to_string(card);
        code += ',';
    }
    code.pop_back();
    return code;
}

int play(deque<int>* cards) {
    unordered_set<string> states;
    while (!cards[0].empty() && !cards[1].empty()) {
        const string state = serialize(cards);
        if (states.find(state) != states.end()) {
            return 0;
        }
        states.emplace(state);
        int draw[2] = {cards[0].front(), cards[1].front()};
        cards[0].pop_front();
        cards[1].pop_front();
        int winner = draw[0] < draw[1];
        if (draw[0] <= cards[0].size() && draw[1] <= cards[1].size()) {
            deque<int> subgame[2] = {deque<int>(cards[0].begin(), cards[0].begin() + draw[0]),
                                     deque<int>(cards[1].begin(), cards[1].begin() + draw[1])};
            winner = play(subgame);
        }
        int loser = 1 - winner;
        cards[winner].emplace_back(draw[winner]);
        cards[winner].emplace_back(draw[loser]);
    }
    return cards[0].empty();
}

void solution() {
    deque<int> cards[2];
    string line;
    for (int player = 0; player < 2; ++player) {
        getline(cin, line);
        while (getline(cin, line) && !line.empty()) {
            cards[player].emplace_back(stoi(line));
        }
    }
    int winner = play(cards);
    int score = 0;
    while (!cards[winner].empty()) {
        score += cards[winner].front() * cards[winner].size();
        cards[winner].pop_front();
    }
    cout << score << '\n';
}
