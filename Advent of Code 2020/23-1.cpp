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

constexpr int NUM_CUPS = 9;
constexpr int NUM_MOVES = 100;

struct Cup {
    int value;
    Cup* next;
};

Cup* seek(Cup* cup, int index) {
    for (int i = 0; i < index; ++i) {
        cup = cup->next;
    }
    return cup;
}

void solution() {
    string inputs;
    cin >> inputs;
    Cup* prev = nullptr;
    Cup* head = nullptr;
    for (char input : inputs) {
        int value = input - '0';
        Cup* cup = new Cup{value, nullptr};
        if (prev != nullptr) {
            prev->next = cup;
        }
        if (head == nullptr) {
            head = cup;
        }
        prev = cup;
    }
    prev->next = head;

    for (int i = 0; i < NUM_MOVES; ++i) {
        int target = 1 + (head->value + NUM_CUPS - 2) % NUM_CUPS;
        Cup* picked = head->next;
        while (seek(picked, 0)->value == target ||
               seek(picked, 1)->value == target ||
               seek(picked, 2)->value == target) {
            target = 1 + (target + NUM_CUPS - 2) % NUM_CUPS;
        }

        head->next = seek(head, 4);

        Cup* tail = head->next;
        while (tail->value != target) {
            tail = tail->next;
        }
        seek(picked, 2)->next = tail->next;
        tail->next = picked;
        head = head->next;
    }

    while (head->value != 1) {
        head = head->next;
    }
    for (Cup* cup = head->next; cup != head; cup = cup->next) {
        cout << cup->value;
    }
    cout << '\n';
}
