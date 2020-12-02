#include <bits/stdc++.h>

using namespace std;

// Common Utilities
//______________________________________________________________________________

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

// Output stream overloads.
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
    out << '[';
    for (int i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i < v.size() - 1) {
            out << ", ";
        }
    }
    return out << ']';
}

template <typename T>
ostream& operator<<(ostream& out, const deque<T>& d) {
    if (d.size() == 0) {
        return out << "[]";
    }

    out << '[' << d[0];
    for (int i = 1; i < d.size(); ++i) {
        out << ' ' << d[i];
    }
    return out << ']';
}

template <typename T>
ostream& operator<<(ostream& out, const set<T>& s) {
    if (s.size() == 0) {
        return out << "{}";
    }

    out << '{' << *s.begin();
    for (auto it = ++s.begin(); it != s.end(); ++it) {
        out << ' ' << *it;
    }
    return out << '}';
}

template <typename T>
ostream& operator<<(ostream& out, const multiset<T>& s) {
    return out << vector<T>(s.begin(), s.end());
}

template <typename T>
ostream& operator<<(ostream& out, const unordered_set<T>& s) {
    if (s.size() == 0) {
        return out << "{}";
    }

    vector<T> v(s.begin(), s.end());
    sort(v.begin(), v.end());

    out << '{' << v[0];
    for (int i = 1; i < v.size(); ++i) {
        out << ' ' << v[i];
    }
    return out << '}';
}

template <typename K, typename V>
ostream& operator<<(ostream& out, const map<K, V>& m) {
    if (m.size() == 0) {
        return out << "{}";
    }

    out << "{\n";
    for (const auto& p : m) {
        const K& key = p.first;
        out << "    " << key << ": " << m.at(key) << '\n';
    }
    return out << '}';
}

template <typename K, typename V>
ostream& operator<<(ostream& out, const unordered_map<K, V>& m) {
    if (m.size() == 0) {
        return out << "{}";
    }

    vector<K> keys;
    for (const auto& p : m) {
        keys.push_back(p.first);
    }
    sort(keys.begin(), keys.end());

    out << "{\n";
    for (const auto& key : keys) {
        out << "    " << key << ": " << m.at(key) << '\n';
    }
    return out << '}';
}

template <typename F, typename S>
ostream& operator<<(ostream& out, const pair<F, S>& p) {
    return out << '(' << p.first << ", " << p.second << ')';
}

template<typename T, typename U>
struct hash<pair<T, U>> {
    size_t operator()(const pair<T, U>& p) const noexcept {
        return hash<T>{}(p.first) ^ (hash<U>{}(p.second) << 1);
    }
};

void solution();

// Initalize the execution environment and call the solution function.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    solution();
    return 0;
}

// Solution
// _____________________________________________________________________________

string repeat_to_str(int reps) {
    switch (reps) {
        case 2: return "double";
        case 3: return "triple";
        case 4: return "quadruple";
        case 5: return "quintuple";
        case 6: return "sextuple";
        case 7: return "septuple";
        case 8: return "octuple";
        case 9: return "nonuple";
        case 10: return "decuple";
        default: return "";
    }
}

string digit_to_str(char digit) {
    switch (digit) {
        case '0': return "zero";
        case '1': return "one";
        case '2': return "two";
        case '3': return "three";
        case '4': return "four";
        case '5': return "five";
        case '6': return "six";
        case '7': return "seven";
        case '8': return "eight";
        case '9': return "nine";
        default: return "Error";
    }
}

void solution() {
    cout << fixed << setprecision(8);
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        string S, F;
        cin >> S >> F;
        F += '-';
        vector<int> fmt;
        int beg = 0;
        int end = F.find_first_of('-');
        while (end != string::npos) {
            fmt.emplace_back(stoi(F.substr(beg, end - beg)));
            beg = end + 1;
            end = F.find_first_of('-', end + 1);
        }
        const auto transcribe = [](const string&& part) -> string {
            string segment;
            for (int i = 0; i < part.size(); ++i) {
                int j = i;
                while (j < part.size() && part[i] == part[j]) {
                    ++j;
                }
                int reps = j - i;
                if (2 <= reps && reps <= 10) {
                    segment += repeat_to_str(reps);
                    segment += ' ';
                }
                if (reps <= 10) {
                    segment += digit_to_str(part[i]);
                    segment += ' ';
                } else {
                    for (int k = 0; k < reps; ++k) {
                        segment += digit_to_str(part[i]);
                        segment += ' ';
                    }
                }
                i = j - 1;
            }
            if (!segment.empty()) {
                segment.pop_back();
            }
            return segment;
        };
        int start = 0;
        string phone;
        for (int size : fmt) {
            phone += transcribe(S.substr(start, size));
            phone += ' ';
            start += size;
        }
        phone.pop_back();
        cout << "Case #" << t << ": " << phone << '\n';
    }
}
