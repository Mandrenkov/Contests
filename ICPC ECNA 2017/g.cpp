// Author: Mikhail Andrenkov
// Date: November 4, 2017
// Problem: ICPC ECNA 2017 [G]
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

// List of calories in each course.
vector<long long> courses;
// Mapping between solve() parameters and solve() return values.
unordered_map<long long, long long> dp;
// The number of courses.
int N;
// The original calorie capacity.
int M;


// Functions

// Returns the most calories that can be consumed from the course index |i| onwards
// given that |skipped| meals were previously skipped at a capacity of |cap|.
long long solve(long long i, long long skipped, long long cap) {
	long long h = (i << 48) + (skipped << 32) + cap;

	if (i == N) {
		return 0;
	} else if (dp.find(h) != dp.end()) {
		return dp[h];
	}

	// The current course may have more calories than the allowed capacity.
	long long cal = min(courses[i], skipped == 0 ? cap*2/3 : cap);

	long long best = 0;
	if (skipped == 0) {
		best = max(solve(i + 1, 0, cap*2/3) + cal, solve(i + 1, 1, cap));
	} else {
		best = max(solve(i + 1, 0, cap) + cal, solve(i + 1, 2, M));
	}

	dp[h] = best;
	return best;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N >> M;

	courses.resize(N);
	for (int i = 0; i < N; ++i) {
		cin >> courses[i];
	}

	cout << solve(0, 2, M) << '\n';

	return 0;
}