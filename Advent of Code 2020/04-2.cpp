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

// Output Stream Overloads
template<
    // Multiple template arguments may be needed for containers like std::map.
    template<class...> class Range, class... Ts,
    // This overload must be disabled for std::string to avoid ambiguity.
    typename = std::enable_if_t<!std::is_same_v<Range<Ts...>, std::string>>
>
std::ostream& operator<<(std::ostream& out, const Range<Ts...>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (std::next(it) != range.end() ? ", " : "");
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

// Reads a single integer from std::cin, advancing the cursor to the next line.
int read_integer() {
    string line;
    getline(cin, line);
    return stoi(line);
}

// Reads a single passport from std::cin, advancing the cursor to the next line.
vector<pair<string, string>> read_passport() {
    string line;
    vector<pair<string, string>> passport;
    while (getline(cin, line) && !line.empty()) {
        const auto tokens = split(line, " ");
        for (const auto& token : tokens) {
            passport.emplace_back(token.substr(0, 3), token.substr(4));
        }
    }
    return passport;
}

// Reports whether the given value is acceptable for the specified field.
bool validate(const string& field, const string& value) {
    // Returns true if |str| represents a number in the range [lo, hi];
    static const auto between = [](const string& str, int lo, int hi) -> bool {
        const int val = stoi(str);
        return lo <= val && val <= hi;
    };

    if (field == "byr") {
        return between(value, 1920, 2002);
    } else if (field == "iyr") {
        return between(value, 2010, 2020);
    } else if (field == "eyr") {
        return between(value, 2020, 2030);
    } else if (field == "hgt") {
        if (value.size() < 4) {
            return false;
        }
        const auto height = value.substr(0, value.size() - 2);
        const auto unit = value.substr(value.size() - 2);
        if (unit == "cm") {
            return between(height, 150, 193);
        } else if (unit == "in") {
            return between(height, 59, 76);
        }
        return false;
    } else if (field == "hcl") {
        if (value.size() != 7 || value[0] != '#') {
            return false;
        }
        return all_of(++value.begin(), value.end(), [](const char c) {
            return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f');
        });
    } else if (field == "ecl") {
        static const vector<string> colours = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        return find(colours.begin(), colours.end(), value) != colours.end();
    } else if (field == "pid") {
        if (value.size() != 9) {
            return false;
        }
        return all_of(value.begin(), value.end(), [](const char c) {
            return '0' <= c && c <= '9';
        });
    }
    return true;
}

void solution() {
    vector<string> fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    int valid = 0;
    int N = read_integer();
    for (int i = 0; i < N; ++i) {
        unordered_set<string> required(fields.begin(), fields.end());
        const auto passport = read_passport();
        for (const auto& [field, value] : passport) {
            const bool valid = validate(field, value);
            if (valid) {
                required.erase(field);
            }
        }
        valid += required.empty();
    }
    cout << valid << '\n';
}
