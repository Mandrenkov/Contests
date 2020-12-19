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

long long eval(const string& expr, const vector<int>& opening, int left, int right) {
    int index, operand;
    if (expr[right] == ')') {
        index = opening[right] - 1;
        operand = eval(expr, opening, index + 2, right - 1);
    } else {
        index = right - 1;
        operand = expr[right] - '0';
    }

    if (index < left) {
        // Only one operand is captured in expr[left..right];
        return operand;
    }

    while (index > left) {
        if (expr[index] == '+') {
            // Add the next operand to the left and repeat.
            if (expr[index - 1] == ')') {
                operand += eval(expr, opening, opening[index - 1] + 1, index - 2);
                index = opening[index - 1] - 1;
            } else {
                operand += expr[index - 1] - '0';
                index -= 2;
            }
        } else if (expr[index] == '*') {
            return operand * eval(expr, opening, left, index - 1);
        } else {
            cerr << "Error: unexpected operation '" << expr[index] << "'.\n";
            return -1;
        }
    }
    return operand;
}

void solution() {
    long long sum = 0;
    string line;
    while (getline(cin, line) && !line.empty()) {
        string expr;
        for (const char c : line) {
            if (c != ' ') {
                expr += c;
            }
        }
        vector<int> opening(expr.size(), -1);
        vector<int> stack;
        for (int i = 0; i < expr.size(); ++i) {
            if (expr[i] == '(') {
                stack.emplace_back(i);
            } else if (expr[i] == ')') {
                opening[i] = stack.back();
                stack.pop_back();
            }
        }
        sum += eval(expr, opening, 0, expr.size() - 1);
    }
    cout << sum << '\n';
}
