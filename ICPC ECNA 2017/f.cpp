// Author: Mikhail Andrenkov
// Date: November 4, 2017
// Problem: ICPC ECNA 2017 [F]
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

long long pairsum(vector<long long>&);

// Classes

// Junction represents a rail line junction.
struct Junction {
	// The ID of this Junction.
	int id;
	// List of Junctions that are directly connected to this Junction.
	vector<Junction*> adj;
	// Map that associates cut Junctions with leftover component sizes.
	unordered_map<Junction*, int> sizes;
	// Number of disconnected Junction pairs when this Junction is removed.
	long long pairs;

	// Determines the size of the connected component when the parent Junction is cut.
	int down(Junction* parent = nullptr) {
		int sum = 1;
		for (const auto& j : adj) {
			if (j != parent) {
				sum += j->down(this);
			}
		}
		sizes[parent] = sum;
		return sum;
	}

	// Determines the size of the connected components when a child Junction is cut.
	void up(Junction* parent = nullptr) {
		int sum = 1;
		for (auto& j : adj) {
			sum += j->sizes[this];
		}

		for (auto& j : adj) {
			sum -= j->sizes[this];
			sizes[j] = sum;
			sum += j->sizes[this];
		}

		for (auto& j : adj) {
			if (j != parent) {
				j->up(this);
			}
		}
	}

	// Calculates the number of disconnected pairs when this Junction is removed.
	long long calculate() {
		vector<long long> comps;
		for (const auto& j : adj) {
			int comp = j->sizes[this];
			comps.push_back(comp);
		}
		pairs = pairsum(comps);
		return pairs;
	}
};


// Declarations


// Functions

// Describes how to display a Junction in an output stream.
ostream& operator<<(ostream& out, const Junction& j) {
	out << "(" << j.id << ":";
	for (const auto& j2 : j.adj) {
		out << " " << j2->id;
	}
	return out << ")";
}

// Determines the pairwise sum of the given connected component sizes.
long long pairsum(vector<long long>& comps) {
	long long sum = 0;
	vector<long long> sums(comps.size());
	for (int i = comps.size() - 1; i >= 0; --i) {
		sum += comps[i];
		sums[i] = sum;
	}

	long long pairs = 0;
	for (int i = 1; i < comps.size(); ++i) {
		pairs += comps[i - 1]*sums[i];
	}
	return pairs;
}

// Execution entry point.
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int N;
	cin >> N;

	vector<Junction*> junctions;
	for (int i = 0; i <= N; ++i) {
		Junction* j = new Junction{i};
		junctions.push_back(j);
	}

	for (int i = 0; i < N; ++i) {
		int j1, j2;
		cin >> j1 >> j2;

		junctions[j1]->adj.push_back(junctions[j2]);
		junctions[j2]->adj.push_back(junctions[j1]);
	}

	// Compute the sizes of all possible connected components.
	Junction* root = junctions[0];
	root->down();
	root->up();

	// Determine the critical junction and the number of disconnected junctions.
	Junction* crit_junction = nullptr;
	long long crit_pairs = 0;
	for (auto& j : junctions) {
		long long pairs = j->calculate();
		if (pairs > crit_pairs) {
			crit_junction = j;
			crit_pairs = pairs;
		}
	}
	assert(crit_junction != nullptr);

	// Connect the two largest components of the critical Junction and calculate the
	// new number of disconnected Junctions.
	vector<long long> comps;
	for (const auto& j : crit_junction->adj) {
		int comp = j->sizes[crit_junction];
		comps.push_back(comp);
	}
	sort(comps.begin(), comps.end());
	comps[comps.size() - 2] += comps.back();
	comps.pop_back();
	long long new_pairs = pairsum(comps);

	cout << crit_pairs << " " << new_pairs << '\n';

	for (auto& j : junctions) {
		delete j;
	}

	return 0;
}