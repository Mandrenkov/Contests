// Author: Mikhail Andrenkov
// Date: November 5, 2017
// Problem: ICPC ECNA 2017 [I]
//____________________________________________________________

#include <cmath>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// Common Utilities
//____________________________________________________________

// Constants
constexpr bool debug = false;
constexpr float EPSILON = 1E-6;
constexpr long long MOD = 1000000007;
constexpr int MAX_INT = 2147483647;
constexpr long long MAX_LONG = 9223372036854775807;

// Macros
#define _pop pop_back
#define _push emplace_back
#define _top back

// Functions
template <typename T>
ostream& operator<<(ostream& out, const vector<T> &v) {
    if (v.size() == 0) {
        return out << "[]";
    }

    out << '[' << v[0];
    for (int i = 1; i < v.size(); ++i) {
        out << ' ' << v[i];
    }
    return out << ']';
}


// Solution
//____________________________________________________________

// Classes

// Expression represents a quadnomial arithmetic expression of the form |a| <op1> |b| <op2> |c| <op3> |d|,
// where each <op> is an addition, subtraction, multiplication, or division operation.
// Each derivation of Expression represents an ordering of these operations.
class Expression {
    public:
        // Constructor for the Expression.
        Expression(int a, int b, int c, int d, char op1, char op2, char op3) {
            this->a = a;
            this->b = b;            
            this->c = c;
            this->d = d;
            this->op1 = op1;
            this->op2 = op2;
            this->op3 = op3;
        }

        // Returns 1 if the given subexpression requires the parentheses.  Otherwise, 0 if returned.
        // If op_left is true, the expression is of the form: t_out <op_out> (t_in1 <op_in> t_in2).
        // If op_left is false, the expression is of the form: (t_in1 <op_in> t_in2) <op_out> t_out.
        //
        // Note: This function assumes that the subexpression (with parentheses) is valid.
        static int needed(int t_in1, int t_in2, int t_out, char op_in, char op_out, bool op_left) {
            try {
                // The evaluation of the subexpression using the specified parentheses.
                int yes;
                // The evaluation of the subexpression ignoring the specified parentheses.
                int no;
                // Denotes whether the |no| subexpression is evaluated strictly from left to right.
                bool order;

                if (op_left) {
                    yes = apply(t_out, op_out, apply(t_in1, op_in, t_in2));

                    if (op_out == '+') {
                        order = false;
                    } else if (op_out == '-') {
                        order = op_in == '+' || op_in == '-';
                    } else {
                        order = true;
                    }
                    no = order ? apply(apply(t_out, op_out, t_in1), op_in, t_in2) : apply(t_out, op_out, apply(t_in1, op_in, t_in2));
                } else {
                    yes = apply(apply(t_in1, op_in, t_in2), op_out, t_out);

                    if (op_out == '+') {
                        order = true;
                    } else if (op_out == '-') {
                        order = true;
                    } else {
                        order = op_in == '*' || op_in == '/';
                    }
                    no = order ? apply(apply(t_in1, op_in, t_in2), op_out, t_out) : apply(t_in1, op_in, apply(t_in2, op_out, t_out));
                }

                // If both evaluations result in the same value, the parentheses may safely be ignored.
                return no == yes ? 0 : 1;
            } catch (char const*) {
                // If a division error occurs, the parentheses are required.
                return 1;
            }
        }

        // Returns the result of the given binary arithmetic operation |op| on |x| and |y|.
        static int apply(int x, char op, int y) {
            if (op == '+') {
                return x + y;
            } else if (op == '-') {
                return x - y;
            } else if (op == '*') {
                return x*y;
            } else if (op == '/') {
                if (y == 0 || x % y != 0) {
                    throw "Division error";
                }
                return x/y;
            }
            throw "Unknown operation";
        }

        // Returns the number of inversions (i.e., adjacent swaps) necessary to transform
        // the [|a|, |b|, |c|, |d|] vector into the |src| vector.
        int inversions(vector<int>& src) {
            vector<int> dest = {a, b, c, d};

            // Map that associates a position in the |dest| vector with a target position in the |src| vector.
            vector<int> order(4, -1);

            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    // If duplicate values exist, ensure they are mapped to different target positions.
                    bool occupied = find(order.begin(), order.end(), j) != order.end();
                    if (!occupied && dest[i] == src[j]) {
                        order[i] = j;
                        break;
                    }
                }
            }

            // Count the number of swaps performed by bubble sort.
            int count = 0;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (order[j] > order[j + 1]) {
                        swap(order[j], order[j + 1]);
                        ++count;
                    }
                }
            }
            return count;
        }

        // Returns the value represented by this Expression.
        virtual int evaluate() = 0;
        // Returns the score represented by this Expression.
        virtual int score(vector<int>& src) = 0;

    protected:
        // The values of the terms in the Expression.
        int a, b, c, d;
        // The operations performed in this Expression.
        char op1, op2, op3;
        // The values of the subexpressions in this Expression.
        int v1, v2, v3;
};

// E1 represents an Expression of the form: ((|a| <op1> |b|) <op2> |c|) <op3> |d|.
class E1 : public Expression {
    public:
        using Expression::Expression;

        int evaluate() {
            try {
                v1 = apply(a, op1, b);
                v2 = apply(v1, op2, c);
                v3 = apply(v2, op3, d);
                return v3;
            } catch (char const*) {
                return -1;
            }
        }

        int score(vector<int>& src) {
            int parentheses = needed(a, b, c, op1, op2, false) + needed(v1, c, d, op2, op3, false);
            int inv = inversions(src);
            return parentheses + inv*2;
        }

        friend ostream& operator<< (ostream& out, const E1& e) {
            return out << "((" << e.a << e.op1 << e.b << ")" << e.op2 << e.c << ")" << e.op3 << e.d;
        }
};

// E2 represents an Expression of the form: (|a| <op1> |b|) <op2> (|c| <op3> |d|).
class E2 : public Expression {
    public:
        using Expression::Expression;

        int evaluate() {
            try {
                v1 = apply(a, op1, b);
                v2 = apply(c, op3, d);
                v3 = apply(v1, op2, v2);
                return v3;
            } catch (char const*) {
                return -1;
            }
        }

        int score(vector<int>& src) {
            int parentheses = needed(a, b, v2, op1, op2, false) + needed(c, d, v1, op3, op2, true);
            int inv = inversions(src);
            return parentheses + inv*2;
        }

        friend ostream& operator<< (ostream& out, const E2& e) {
            return out << "(" << e.a << e.op1 << e.b << ")" << e.op2 << "(" << e.c << e.op3 << e.d << ")";
        }
};

// E3 represents an Expression of the form: (|a| <op1> (|b| <op2> |c|)) <op3> |d|.
class E3 : public Expression {
    public:
        using Expression::Expression;

        int evaluate() {
            try {
                v1 = apply(b, op2, c);
                v2 = apply(a, op1, v1);
                v3 = apply(v2, op3, d);
                return v3;
            } catch (char const*) {
                return -1;
            }
        }

        int score(vector<int>& src) {
            int parentheses = needed(b, c, a, op2, op1, true) + needed(a, v1, d, op1, op3, false);
            int inv = inversions(src);
            return parentheses + inv*2;
        }

         friend ostream& operator<< (ostream& out, const E3& e) {
            return out << "(" << e.a << e.op1 << "(" << e.b << e.op2 << e.c << "))" << e.op3 << e.d;
        }
};

// E4 represents an Expression of the form: |a| <op1> ((|b| <op2> |c|) <op3> |d|).
class E4 : public Expression {
    public:
        using Expression::Expression;

        int evaluate() {
            try {
                v1 = apply(b, op2, c);
                v2 = apply(v1, op3, d);
                v3 = apply(a, op1, v2);
                return v3;
            } catch (char const*) {
                return -1;
            }
        }

        int score(vector<int>& src) {
            int parentheses = needed(b, c, d, op2, op3, false) + needed(v1, d, a, op3, op1, true);
            int inv = inversions(src);
            return parentheses + inv*2;
        }

        friend ostream& operator<< (ostream& out, const E4& e) {
            return out << e.a << e.op1 << "((" << e.b << e.op2 << e.c << ")" << e.op3 << e.d << ")";
        }
};

// E5 represents an Expression of the form: |a| <op1> (|b| <op2> (|c| <op3> |d|)).
class E5 : public Expression {
    public:
        using Expression::Expression;

        int evaluate() {
            try {
                v1 = apply(c, op3, d);
                v2 = apply(b, op2, v1);
                v3 = apply(a, op1, v2);
                return v3;
            } catch (char const*) {
                return -1;
            }
        }

        int score(vector<int>& src) {
            int parentheses = needed(c, d, b, op3, op2, true) + needed(b, v1, a, op2, op1, true);
            int inv = inversions(src);
            return parentheses + inv*2;
        }

        friend ostream& operator<< (ostream& out, const E5& e) {
            return out << e.a << e.op1 << "(" << e.b << e.op2 << "(" << e.c << e.op3 << e.d << "))";
        }
};

// Declarations


// Functions

// Inserts all permutations of the elements in |src| into the |perms| vector. 
void permutations(vector<vector<int>>& perms, vector<int>& current, vector<int>& src) {
    if (src.empty()) {
        perms.push_back(vector<int>(current.begin(), current.end()));
        return;
    }

    for (const auto& e : vector<int>(src)) {
        current.push_back(e);
        src.erase(find(src.begin(), src.end(), e));

        permutations(perms, current, src);

        src.push_back(e);
        current.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    vector<int> src(4);
    for (int i = 0; i < 4; ++i) {
        cin >> src[i];
    }

    vector<vector<int>> perms;
    vector<int> current;
    permutations(perms, current, src);

    vector<char> ops =  {'+', '-', '*', '/'};
    int best = MAX_INT;

    // Iterate through each permutation and operation combination.
    for (const auto& op1 : ops) {
        for (const auto& op2 : ops) {
            for (const auto& op3 : ops) {
                for (const auto& p : perms) {
                    Expression* e1 = new E1(p[0], p[1], p[2], p[3], op1, op2, op3);
                    Expression* e2 = new E2(p[0], p[1], p[2], p[3], op1, op2, op3);
                    Expression* e3 = new E3(p[0], p[1], p[2], p[3], op1, op2, op3);
                    Expression* e4 = new E4(p[0], p[1], p[2], p[3], op1, op2, op3);
                    Expression* e5 = new E5(p[0], p[1], p[2], p[3], op1, op2, op3);

                    for (auto& e : {e1, e2, e3, e4, e5}) {
                        if (e->evaluate() == 24) {
                            best = min(best, e->score(src));
                        }

                        // Not freeing this memory may cause some issues down the line.
                        delete e;
                    }
                }
            }
        } 
    }
    
    if (best < MAX_INT) {
        cout << best << '\n';
    } else {
        cout << "impossible\n";
    }

    return 0;
}