#include <bits/stdc++.h>

using namespace std;

// Macros
#ifdef D3BUG
    #define cerrd if (D3BUG) cerr
#else
    #define cerrd if (false) cerr
#endif

// Types
template <typename T> using vector1D = vector<T>;
template <typename T> using vector2D = vector<vector1D<T>>;
template <typename T> using vector3D = vector<vector2D<T>>;

// Output Stream Overloads
template<
    // Multiple template arguments may be needed for containers like std::map.
    template<class...> class Range, class... Ts,
    // This overload must be disabled for std::string to avoid ambiguity.
    typename = std::enable_if_t<!std::is_same_v<Range<Ts...>, std::string>>
>
std::ostream& operator<<(std::ostream& out, const Range<Ts...>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (std::next(it) != range.end() ? ", " : "");
    }
    return out << '}';
}

template <class F, class S>
ostream& operator<<(ostream& out, const pair<F, S>& pair) {
    return out << '(' << pair.first << ", " << pair.second << ')';
}

// Hash Template Specializations
template<class T, class U>
struct hash<pair<T, U>> {
    size_t operator()(const pair<T, U>& p) const noexcept {
        return hash<T>{}(p.first) ^ (hash<U>{}(p.second) << 1);
    }
};

void solution();

// Initalize the execution environment and call the solution function.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    try {
        solution();
    } catch (const char* what) {
        cerr << "Exception: " << what << ".\n";
    }
}

// -----------------------------------------------------------------------------

// Ask for the median of |i|, |j|, and |k|.
int query(int i, int j, int k) {
    int median;
    cout << i << ' ' << j << ' ' << k << endl;
    cin >> median;
    return median;
}

// Find the relative order of the current index using ternary search.
int ternary_search(const vector<int>& A, int index) {
    int lo = 0;
    int hi = A.size() - 1;
    while (lo + 1 < hi) {
        int mid_l = lo + 1 * (hi - lo) / 3;
        int mid_r = max(mid_l + 1, lo + 2 * (hi - lo) / 3);

        int median = query(A[mid_l], A[mid_r], index);
        if (median == A[mid_l]) {
            hi = mid_l;
        } else if (median == A[mid_r]) {
            lo = mid_r;
        } else {
            lo = mid_l;
            hi = mid_r;
        }
    }
    
    // Check if the element comes before the array.
    if (lo == 0 && hi == 1) {
        int median = query(A[lo], A[hi], index);
        if (median == index) {
            return 1;
        } else {
            return 0;
        }
    }

    // Check if the element comes after the array.
    if (lo == A.size() - 2 && hi == A.size() - 1) {
        int median = query(A[lo], A[hi], index);
        if (median == index) {
            return A.size() - 1;
        } else {
            return A.size();
        }
    }

    return hi;
}

void solution() {
    int T, N, Q;
    cin >> T >> N >> Q;
    for (int t = 1; t <= T; ++t) {
        vector<int> A;

        // Any consistent order for the first three elements is acceptable.
        int first = query(1, 2, 3);
        if (first == 1) {
            A = {2, 1, 3};
        } else if (first == 2) {
            A = {1, 2, 3};
        } else {
            A = {1, 3, 2};
        }

        // Insert the remaining elements using ternary search.
        for (int i = 4; i <= N; ++i) {
            int index = ternary_search(A, i);
            A.insert(A.begin() + index, i);
        }

        for (int index : A) {
            cout << index << ' ';
        }
        cout << endl;

        // No need to check for -1: this solution is perfect.
        int verdict;
        cin >> verdict;
    }
}
