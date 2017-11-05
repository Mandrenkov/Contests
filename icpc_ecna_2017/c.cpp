// Author: Mikhail Andrenkov
// Date: November 4, 2017
// Problem: ICPC ECNA 2017 [C]
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

// Returns the sum of the characters in |s| modulo 26. 
int sum(const string& s) {
	int value = 0;
	for (const auto& c : s) {
		value += c - 'A';
	}
	return value % 26;
}

// Execution entry point.
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	string ciphertext;
	cin >> ciphertext;

	int half = ciphertext.size()/2;
	string s1 = ciphertext.substr(0, half);
	string s2 = ciphertext.substr(half);

	int v1 = sum(s1);
	int v2 = sum(s2);

	for (int i = 0; i < half; ++i) {
		int c1 = s1[i] - 'A';
		int c2 = s2[i] - 'A';
		int next = (c1 + c2 + v1 + v2) % 26;
		cout << static_cast<char>(next + 'A');
	}
	cout << '\n';

	return 0;
}