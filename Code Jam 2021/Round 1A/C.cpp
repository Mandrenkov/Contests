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
ostream& operator<<(ostream& out, const pair<F, S>& pair) {
    return out << '(' << pair.first << ", " << pair.second << ')';
}

// Hash Template Specializations
template<class T, class U>
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
}

// -----------------------------------------------------------------------------

using int_t = __int128;

// Computes the absolute value of `n`.
int_t abs(const int_t n) {
    return n < 0 ? -n : n;
}

// Computes the GCD of `x` and `y`.
int_t gcd(int_t x, int_t y) {
    x = abs(x);
    y = abs(y);
    if (x < y) {
        swap(x, y);
    }

    if (y == 0) {
        return 1;
    }

    while (x % y > 0) {
        x = (x - y) % y;
        swap(x, y);
    }

    return y;
}

// Inserts the given integer into the stream.
ostream& operator<<(ostream& out, int_t n) {
    const bool neg = n < 0;

    string s;
    while (n >= 10) {
        s += '0' + (n % 10);
        n /= 10;
    }

    s += '0' + n;
    if (neg) {
        s += '-';
    }

    reverse(s.begin(), s.end());
    return out << s;
}

// Fraction represents a rational number.
struct Fraction {
    int_t top;
    int_t bot;

    Fraction() : top(0), bot(1) {}

    Fraction(int_t top) : top(top), bot(1) {};

    Fraction(int_t top, int_t bot) : top(top), bot(bot) {
        reduce();
    }

    Fraction operator+(const Fraction& other) const {
        return Fraction(top * other.bot + other.top * bot, bot * other.bot);
    }

    Fraction operator-(const Fraction& other) const {
        return Fraction(top * other.bot - other.top * bot, bot * other.bot);
    }

    Fraction operator*(const Fraction& other) const {
        return Fraction(top * other.top, bot * other.bot);
    }

    Fraction operator/(const Fraction& other) const {
        return Fraction(top * other.bot, bot * other.top);
    }

    Fraction& operator++() {
        top += bot;
        reduce();
        return *this;
    }

    Fraction operator-() const {
        return Fraction(-top, bot);
    }


    bool operator<(const Fraction& other) const {
        return top * other.bot < bot * other.top;
    }

    bool operator<=(const Fraction& other) const {
        return top * other.bot <= bot * other.top;
    }

    bool operator==(const Fraction& other) const {
        return top == other.top && bot == other.bot;
    }

    Fraction& operator+=(const Fraction& other) {
        top = top * other.bot + other.top * bot;
        bot = bot * other.bot;
        reduce();
        return *this;
    }

    Fraction& operator*=(const Fraction& other) {
        top *= other.top;
        bot *= other.bot;
        reduce();
        return *this;
    }

    Fraction& operator/=(const Fraction& other) {
        top *= other.bot;
        bot *= other.top;
        reduce();
        return *this;
    }

    // Reports whether this fraction is a whole number.
    bool whole() const {
        if (bot == 0) {
            return false;
        }
        return abs(top) % abs(bot) == 0;
    }

    // Reports whether this fraction is a positive number.
    bool positive() const {
        if (top == 0 || bot == 0) {
            return true;
        }
        const bool top_pos = top >= 0;
        const bool bot_pos = bot >= 0;
        return top_pos == bot_pos;
    }

    // Reduces this fraction.
    void reduce() {
        if (top < 0 && bot < 0) {
            top = -top;
            bot = -bot;
        }

        if (top == 0 && bot != 0) {
            bot = 1;
            return;
        }

        int_t div = gcd(top, bot);
        while (div != 1) {
            top /= div;
            bot /= div;
            div = gcd(top, bot);
        }
    }
};

// Inserts the given fraction into the stream.
ostream& operator<<(ostream& out, const Fraction& fraction) {
    return out << fraction.top << '/' << fraction.bot;
}

// Computes `n` choose `k`.
Fraction choose(int_t n, int_t k) {
    Fraction chosen(1, 1);
    for (int_t i = 0; i < k; ++i) {
        chosen = chosen * Fraction(n - i, k - i);
    }
    return chosen;
}

struct Exam {
    string answers;
    Fraction score;
};

void solution() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, Q;
        cin >> N >> Q;

        vector<Exam> exams(N);
        for (int i = 0; i < N; ++i) {
            int score;
            cin >> exams[i].answers >> score;
            exams[i].score = Fraction(score, 1);
        }

        // Make sure there are always 3 students.
        while (exams.size() < 3) {
            Exam exam = exams.front();
            exams.push_back(exam);
        }

        // "NOT" all the questions to which the first student answered 'F'.
        // Keep track of these questions to flip them back in the final answer.
        vector<bool> flipped(Q);
        for (int i = 0; i < Q; ++i) {
            if (exams[0].answers[i] == 'F') {
                flipped[i] = true;
                exams[0].answers[i] = exams[0].answers[i] == 'T' ? 'F' : 'T';
                exams[1].answers[i] = exams[1].answers[i] == 'T' ? 'F' : 'T';
                exams[2].answers[i] = exams[2].answers[i] == 'T' ? 'F' : 'T';
            }
        }

        Fraction freqs[2][2];
        for (int i = 0; i < Q; ++i) {
            const int j = exams[1].answers[i] == 'T';
            const int k = exams[2].answers[i] == 'T';
            ++freqs[j][k];
        }

        // Probabilities are NOT normalized for numeric precision.
        Fraction probs[2][2];
        // Number of questions of each type (i.e., FF, FT, TF, TT).
        Fraction trues[2][2];
        // Total number of answers that are consistent with the students' results.
        Fraction total;

        for (int_t i = 0; i <= freqs[1][1].top; ++i) {
            trues[1][1] = Fraction(i);

            // Manually solve the system of linear equations.
            const auto r1 = exams[0].score - trues[1][1];
            const auto r2 = exams[1].score - trues[1][1] - freqs[0][0] - freqs[0][1];
            const auto r3 = exams[2].score - trues[1][1] - freqs[0][0] - freqs[1][0];
            trues[0][0] = (r2 + r3) / 2 * -1;
            trues[0][1] = (r1 + r3) / 2;
            trues[1][0] = (r1 + r2) / 2;

            // The number of questions in each category whose correct answer is 'T' must
            // be a whole, positive number, and cannot exceed the number of questions in
            // that category.
            const bool feasible = trues[0][0].whole() && trues[0][0].positive() && trues[0][0] <= freqs[0][0] &&
                                  trues[0][1].whole() && trues[0][1].positive() && trues[0][1] <= freqs[0][1] &&
                                  trues[1][0].whole() && trues[1][0].positive() && trues[1][0] <= freqs[1][0];
            if (!feasible) {
                continue;
            }

            // Compute the number of answers that are consistent with these counts.
            Fraction current(1, 1);
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    current *= choose(freqs[i][j].top, trues[i][j].top);
                }
            }

            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    probs[i][j] += current * trues[i][j];
                }
            }

            total = total + current;
        }

        Fraction expval(0, 1);
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                const auto& prob_T = probs[i][j];
                const auto& prob_F = total * freqs[i][j] - probs[i][j];
                // For each question category, choose the more likely answer.
                if (prob_T < prob_F) {
                    expval += prob_F;
                } else {
                    expval += prob_T;
                }
            }
        }
        expval /= total;

        string answer;
        for (int i = 0; i < Q; ++i) {
            const int j = exams[1].answers[i] == 'T';
            const int k = exams[2].answers[i] == 'T';
            // Do the same thing as the previous loop, but now append to the answer key.
            const auto& prob_T = probs[j][k];
            const auto& prob_F = total * freqs[j][k] - probs[j][k];
            if (prob_T < prob_F) {
                answer += 'F';
            } else {
                answer += 'T';
            }
        }

        for (int i = 0; i < Q; ++i) {
            if (flipped[i]) {
                answer[i] = answer[i] == 'T' ? 'F' : 'T';
            }
        }

        cout << "Case #" << t << ": " << answer << ' ' << expval << "\n";
    }
}
