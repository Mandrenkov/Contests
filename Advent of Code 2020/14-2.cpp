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

void solution() {
    int N;
    cin >> N;
    string bitmask;
    unordered_map<bitset<36>, long long> memory;
    for (int i = 0; i < N; ++i) {
        string tokens[3];
        cin >> tokens[0] >> tokens[1] >> tokens[2];
        if (tokens[0].substr(0, 3) == "mem") {
            // Update the memory.
            bitset<36> address = stoi(tokens[0].substr(4, tokens[0].size() - 5));
            const long long value = stoll(tokens[2]);
            const function<void(bitset<36>&, int)> decode = [&](bitset<36>& address, int index) {
                if (index == 36) {
                    memory[address] = value;
                } else if (bitmask[index] == '0') {
                    // Leave the memory address bit unchanged.
                    decode(address, index + 1);
                } else if (bitmask[index] == '1') {
                    // Set the memory address bit to 1.
                    int bit = 35 - index;
                    address.set(bit, true);
                    decode(address, index + 1);
                } else {
                    // Set the memory address bit to 0 and then 1.
                    int bit = 35 - index;
                    address.set(bit, false);
                    decode(address, index + 1);
                    address.set(bit, true);
                    decode(address, index + 1);
                }
            };
            decode(address, 0);
        } else {
            // Update the bitmask.
            bitmask = tokens[2];
        }
    }
    long long sum = 0;
    for (const auto [_, value] : memory) {
        sum += value;
    }
    cout << sum << '\n';
}
