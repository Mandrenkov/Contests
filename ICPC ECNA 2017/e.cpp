// Author: Mikhail Andrenkov
// Date: November 4, 2017
// Problem: ICPC ECNA 2017 [E]
//____________________________________________________________

#include <iostream>
#include <map>
#include <math.h>
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
constexpr float EPSILON = 1E-10;
constexpr long long MOD = 1000000007;
constexpr int MAX_INT = 2147483647;
constexpr long long MAX_LONG = 9223372036854775807;

// Macros
#define _pop pop_back
#define _push emplace_back
#define _top back

// Functions
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
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

// Type represents a class in ICPC++.
struct Type {
	// The name of the Type.
	string name;
	// All parents in the Type hierarchy.
	unordered_set<Type*> is_a;
	// All Types that this Type directly possesses.
	unordered_set<Type*> has_a;
	// Denotes whether this Type has been processed.
	bool resolved;

	// Constructs a new Type object with the given name.
	Type(const string name) : name(name), resolved(false) {}

	// Resolves the parent hierarchy of this Type.
	void up() {
		if (resolved) return;

		for (auto& t : is_a) {
			t->up();
		}

		unordered_set<Type*> is_a_copy = is_a;
		for (auto& t : is_a_copy) {
			for (auto& t2 : t->is_a) {
				is_a.insert(t2);
			}
		}
		is_a.insert(this);
		
		resolved = true;
	}

	// Adds the given Type to the provided output stream and returns the stream.  
	friend ostream& operator<< (ostream& out, const Type& t) {
		out << t.name << ": {";
		for (const auto& t2 : t.is_a) {
			out << " " << t2->name;
		}
		out << " }, {";
		for (const auto& t2 : t.has_a) {
			out << " " << t2->name;
		}
		return out << " }";
	}
};

// Declarations

// Functions
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int N, Q;
	cin >> N >> Q;

	unordered_map<string, Type*> types;
	for (int i = 0; i < N; ++i) {
		string t1, r, t2;
		cin >> t1 >> r >> t2;

		if (types.find(t1) == types.end()) {
			types[t1] = new Type(t1);
		}
		if (types.find(t2) == types.end()) {
			types[t2] = new Type(t2);
		}

		if (r == "is-a") {
			types[t1]->is_a.insert(types[t2]);
		} else if (r == "has-a") {
			types[t1]->has_a.insert(types[t2]);
		} else {
			cerr << "1. Unknown relationship \"" << r << "\".\n";
			return 1;
		}
	}

	// Resolve the parent hierarchy of each Type.
	for (const auto& p : types) {
		p.second->up();
	}

	for (int q = 1; q <= Q; ++q) {
		string s1, r, s2;
		cin >> s1 >> r >> s2;

		Type* t1 = types[s1];
		Type* t2 = types[s2];

		cout << "Query " << q << ": ";

		if (r == "is-a") {
			// Check the parent hierarchy of |t1| for |t2|.
			cout << (t1->is_a.find(t2) != t1->is_a.end() ? "true" : "false") << '\n';
		} else if (r == "has-a") {
			unordered_set<Type*> prev;

			// Find all the Types that |t1| (or its parents) have.
			unordered_set<Type*> next;
			for (const auto& t : t1->is_a) {
				for (const auto& t2 : t->has_a) {
					next.insert(t2);
				}
			}
			bool found = next.find(t2) != next.end();

			// Iteratively search the Type is-a/has-a graph until
			// |t2| is discovered or no new information can be gained.
			while (!found && prev.size() != next.size()) {
				prev = next;
				for (const auto& t : prev) {
					for (const auto& t2 : t->is_a) {
						next.insert(t2);
					}
					for (const auto& t2 : t->has_a) {
						next.insert(t2);
					}
				}
				found = next.find(t2) != next.end();
			}
			cout << (found ? "true" : "false") << '\n';
		} else {
			cerr << "2. Unknown relationship \"" << r << "\".\n";
			return 1;
		}
	}

	for (auto& p : types) {
		delete p.second;
	}

	return 0;
}