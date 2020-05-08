// Author: Mikhail Andrenkov
// Date: November 5, 2017
// Problem: ICPC ECNA 2017 [H]
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


// Declarations

// The number of rows in the petri dish.
int R;
// The number of columns in the petri dish.
int C;
// Stores the contents of the cells in the dish.
vector<vector<bool>> dish;
// Stores the visitation of the cells in the dish.
vector<vector<bool>> visited;

// Functions

// Visits all cells belonging to the amoeba ring with a component at the given row and column.
void dfs(int r, int c) {
	visited[r][c] = true;

	int r_min = max(0, r - 1);
	int r_max = min(R - 1, r + 1);

	int c_min = max(0, c - 1);
	int c_max = min(C - 1, c + 1);

	for (int r2 = r_min; r2 <= r_max; ++r2) {
		for (int c2 = c_min; c2 <= c_max; ++c2) {
			if (dish[r2][c2] && !visited[r2][c2]) {
				dfs(r2, c2);
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> R >> C;

	dish.resize(R);
	for (int r = 0; r < R; ++r) {
		string s;
		cin >> s;

		dish[r].resize(C);
		for (int c = 0; c < C; ++c) {
			dish[r][c] = s[c] == '#';
		}
	}

	// Initially, no cells in the dish are visited.
	visited.resize(R);
	for (auto& row : visited) {
		row.resize(C, false);
	}

	int amoebas = 0;
	for (int r = 0; r < R; ++r) {
		for (int c = 0; c < C; ++c) {
			// Check if the current cell is part of an undiscovered amoeba.
			if (dish[r][c] && !visited[r][c]) {
				dfs(r, c);
				++amoebas;
			}
		}
	}

	cout << amoebas << '\n';

	return 0;
}