// Author: Mikhail Andrenkov
// Date: November 5, 2017
// Problem: ICPC ECNA 2017 [J]
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

// Person represents the usage pattern of a person (other than Jim) using a machine.
struct Person {
    // The time when the person wants to start using the machine. 
    long long start;
    // The duration of the person's workout.
    long long duration;
    // The duration of the person's recovery.
    long long recovery;
    // The duration added to the recovery time.
    long long cycle;
    // The previous moment in time when the person started using the machine.
    long long last_start;
};

// Machine represents Jim's usage and recovery time for a machine.
struct Machine {
    // The duration of the Jim's workout.
    long long duration;
    // The duration of the Jim's recovery.
    long long recovery;
};


// Declarations

// Functions
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    vector<Machine> machines(10);
    for (int i = 0; i < 10; ++i) {
        long long u, r;
        cin >> u >> r;
        machines[i] = Machine{u, r};
    }

    vector<Person> people(10);
    for (int i = 0; i < 10; ++i) {
        long long u, r, t;
        cin >> u >> r >> t;
        people[i] = Person{t, u, r, u + r, t};
    }

    long long t = 0;
    for (int s = 0; s < 3; ++s) {
        for (int m = 0; m < 10; ++m) {
            Machine& machine = machines[m];
            Person& person = people[m];

            // The other person is willing to use the machine.
            if (t >= person.start) {
                // Compute the last time the person started to use the machine.
                long long elapsed = (t - person.last_start) % person.cycle;
                long long last_start = t - elapsed;

                // Wait if the person is currently using the machine.
                if (elapsed < person.duration) {
                    t += person.duration - elapsed;
                    elapsed = person.duration;
                }

                // Jim uses the machine.
                t += machine.duration;

                // Calculate the next time the person can use the machine.
                if (t >= last_start + person.cycle) {
                    last_start = t;
                }
                person.last_start = last_start;

                // Jim must recover after using the machine.
                t += machine.recovery;
            // The other person is not yet willing to use the machine.
            } else {
                // Jim uses the machine.
                t += machine.duration;

                // If the person now wants to use the machine, update their last usage start time. 
                if (t >= person.start) {
                    person.last_start = t;
                }

                // Jim must recover after using the machine.
                t += machine.recovery;
            }
        }
    }

    // Jim does not need to recover after the last workout.
    t -= machines[9].recovery;

    cout << t << '\n';

    return 0;
}