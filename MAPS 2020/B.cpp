#include <bits/stdc++.h>

using namespace std;

using uint128 = unsigned __int128;

uint128 pow(const uint128 base, const uint128 exp, const uint128 mod) {
    uint128 build = base % mod;
    uint128 power = 1;
    for (uint128 mask = 1; mask <= exp; mask <<= 1) {
        if (exp & mask) {
            power = (power * build) % mod;
        }
        build = (build * build) % mod;
    }
    return power;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    unsigned long long B, D;
    cin >> B >> D;

    bool exists = (D == 2) && (B % 2 == 1);
    unsigned long long period = D - 1;
    while (!exists && period > 0 && period % 2 == 0) {
        period /= 2;
        exists = pow(B, period, D) == D - 1;
    }
    cout << (exists ? "yes" : "no") << '\n';

    return 0;
}