#include <bits/stdc++.h>

using namespace std;

string readLine() {
    string line;
    getline(cin, line);
    return line;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N = stoi(readLine());
    unordered_set<string> keywords;
    for (int i = 0; i < N; ++i) {
        string phrase = readLine();
        replace(phrase.begin(), phrase.end(), '-', ' ');
        transform(phrase.begin(), phrase.end(), phrase.begin(), [](unsigned char c) { return tolower(c); });
        keywords.insert(phrase);
    }
    cout << keywords.size() << '\n';

    return 0;
}