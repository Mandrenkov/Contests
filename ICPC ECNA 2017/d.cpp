// Author: Mikhail Andrenkov
// Date: November 4, 2017
// Problem: ICPC ECNA 2017 [D]
//____________________________________________________________

#include <cassert>
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


// Declarations


// Functions
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int N, K;
	cin >> N >> K;

	vector<int> pos = {0};
	for (int k = 0; k < K; ++k) {
		string s;
		cin >> s;

		if (s != "undo") {
			int t = stoi(s);
			int next = ((pos.back() + t) % N + N) % N;
			pos.push_back(next);
		} else {
			int undo;
			cin >> undo;
			pos.erase(pos.end() - undo, pos.end());
		}
	}

	cout << pos.back() << '\n';

	return 0;
}