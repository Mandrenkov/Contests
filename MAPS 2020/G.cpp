#include <bits/stdc++.h>

using namespace std;

// Checks if |prefix| a prefix of S starting from index |index|.
bool check(const string& S, int& index, const string&& prefix) {
    const bool match = S.substr(index, prefix.size()) == prefix;
    if (match) {
        index += prefix.size();
    }
    return match;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    vector<long long> stack;

    string S;
    cin >> S;
    for (int index = 0; index < S.size(); ) {
        if (check(S, index, "SS")) {
            // Parse integer
            bool positive = S[index++] == 'S';
            long long number = 0;
            while (S[index] != 'N') {
                number <<= 1;
                number += S[index++] == 'S' ? 0 : 1;
            }
            stack.push_back(positive ? number : -number);
            ++index;
        } else if (check(S, index, "SNS")) {
            // Copy
            if (!stack.empty()) {
                stack.push_back(stack.back());
            } else {
                cout << "Invalid copy operation\n";
            }
        } else if (check(S, index, "SNT")) {
            // Swap
            if (stack.size() >= 2) {
                swap(stack[stack.size() - 2], stack[stack.size() - 1]);
            } else {
                cout << "Invalid swap operation\n";
            }
        } else if (check(S, index, "SNN")) {
            // Remove
            if (!stack.empty()) {
                stack.pop_back();
            } else {
                cout << "Invalid remove operation\n";
            }
        } else if (check(S, index, "TSSS")) {
            // Addition
            if (stack.size() >= 2) {
                stack[stack.size() - 2] += stack.back();
                stack.pop_back();
            } else {
                cout << "Invalid addition operation\n";
            }
        } else if (check(S, index, "TSST")) {
            // Subtraction
            if (stack.size() >= 2) {
                stack[stack.size() - 2] -= stack.back();
                stack.pop_back();
            } else {
                cout << "Invalid subtraction operation\n";
            }
        } else if (check(S, index, "TSSN")) {
            // Multiplication
            if (stack.size() >= 2) {
                stack[stack.size() - 2] *= stack.back();
                stack.pop_back();
            } else {
                cout << "Invalid multiplication operation\n";
            }
        } else if (check(S, index, "TSTS")) {
            // Division
            if (stack.size() >= 2 && stack.back() != 0) {
                stack[stack.size() - 2] /= stack.back();
                stack.pop_back();
            } else if (stack.size() < 2) {
                cout << "Invalid division operation\n";
            } else if (stack.back() == 0) {
                cout << "Division by zero\n";
            }
        } else if (check(S, index, "TNST")) {
            if (!stack.empty()) {
                cout << stack.back() << '\n';
                stack.pop_back();
            } else {
                cout << "Invalid print operation\n";
            }
        } else {
            cerr << "Unknown instruction prefix \"" << S.substr(index) << "\".\n";
            break;
        }
    }
    return 0;
}