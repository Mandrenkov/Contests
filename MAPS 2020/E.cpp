#include <bits/stdc++.h>

using namespace std;

inline long long ATP(long long cost, long long tax) {
    // c * (1 + t / (100 * 100)) + (1 / 2)
    // c * (1 + t / (10000)) + (1 / 2)
    // (c * (10000 + t) / 10000) + (1 / 2)
    // (c * (10000 + t) / 10000) + (5000 / 10000)
    // (c * (10000 + t) + 5000) / 10000
    return (cost * (10000LL + tax) + 5000LL) / 10000LL;
}

long long parse(const string& currency) {
    string head = currency.substr(0, currency.length() - 3);
    string tail = currency.substr(currency.length() - 2);
    long long dollars = stoll(head);
    long long cents = stoll(tail);
    return 100LL * dollars + cents;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;
    while (N --> 0) {
        string a, b, t;
        cin >> a >> b >> t;

        long long A = parse(a);
        long long B = parse(b);
        long long T = parse(t);

        long long period = 10000LL + T;
        vector<long long> skipped;
        vector<bool> hits(period, false);
        for (long long cost = 0; cost < period; ++cost) {
            long long after = ATP(cost, T);
            if (after < period) {
                hits[after] = true;
            }
        }
        for (long long c = 0; c < period; ++c) {
            if (!hits[c]) {
                skipped.emplace_back(c);
            }
        }

        long long rem_A = A % period;
        long long rem_B = B % period;
        long long between = B / period - A / period;
        long long count = 0;
        if (between == 0) {
            for (long long skip : skipped) {
                if (rem_A <= skip && skip <= rem_B) {
                    ++count;
                }
            }
        } else {
            long long count_A = 0;
            for (long long skip : skipped) {
                if (skip >= rem_A) {
                    ++count_A;
                }
            }
            long long count_B = 0;
            for (long long skip : skipped) {
                if (skip <= rem_B) {
                    ++count_B;
                }
            }
            long long count_C = (between - 1) * skipped.size();
            count = count_A + count_B + count_C;
        }
        cout << count << '\n';
    }

    return 0;
}