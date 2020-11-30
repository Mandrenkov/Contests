#include <bits/stdc++.h>

using namespace std;

// Common Utilities
//______________________________________________________________________________

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
template <typename T> using uset = unordered_set<T>;
template <typename K, typename V> using umap = unordered_map<K, V>;

// Output stream overloads.
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
    out << '[';
    for (int i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i < v.size() - 1) {
            out << ", ";
        }
    }
    return out << ']';
}

template <typename T>
ostream& operator<<(ostream& out, const vector<T*>& v) {
    out << '[';
    for (int i = 0; i < v.size(); ++i) {
        out << *v[i];
        if (i < v.size() - 1) {
            out << ", ";
        }
    }
    return out << ']';
}

template <typename T>
ostream& operator<<(ostream& out, const deque<T>& d) {
    if (d.size() == 0) {
        return out << "[]";
    }

    out << '[' << d[0];
    for (int i = 1; i < d.size(); ++i) {
        out << ' ' << d[i];
    }
    return out << ']';
}

template <typename T>
ostream& operator<<(ostream& out, const set<T>& s) {
    if (s.size() == 0) {
        return out << "{}";
    }

    out << '{' << *s.begin();
    for (auto it = ++s.begin(); it != s.end(); ++it) {
        out << ' ' << *it;
    }
    return out << '}';
}

template <typename T>
ostream& operator<<(ostream& out, const multiset<T>& s) {
    return out << vector<T>(s.begin(), s.end());
}

template <typename T>
ostream& operator<<(ostream& out, const unordered_set<T>& s) {
    if (s.size() == 0) {
        return out << "{}";
    }

    vector<T> v(s.begin(), s.end());
    sort(v.begin(), v.end());

    out << '{' << v[0];
    for (int i = 1; i < v.size(); ++i) {
        out << ' ' << v[i];
    }
    return out << '}';
}

template <typename K, typename V>
ostream& operator<<(ostream& out, const map<K, V>& m) {
    if (m.size() == 0) {
        return out << "{}";
    }

    out << "{\n";
    for (const auto& p : m) {
        const K& key = p.first;
        out << "    " << key << ": " << m.at(key) << '\n';
    }
    return out << '}';
}

template <typename K, typename V>
ostream& operator<<(ostream& out, const unordered_map<K, V>& m) {
    if (m.size() == 0) {
        return out << "{}";
    }

    vector<K> keys;
    for (const auto& p : m) {
        keys.push_back(p.first);
    }
    sort(keys.begin(), keys.end());

    out << "{\n";
    for (const auto& key : keys) {
        out << "    " << key << ": " << m.at(key) << '\n';
    }
    return out << '}';
}

template <typename F, typename S>
ostream& operator<<(ostream& out, const pair<F, S>& p) {
    return out << '(' << p.first << ", " << p.second << ')';
}

template<typename T, typename U>
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
    solution();
    return 0;
}

// Solution
// _____________________________________________________________________________

constexpr int BITS = 17;

struct Node {
    int index;
    int value;
    int size;
    Node* parent;
    Node* kids[2];

    Node() : index(0), value(0), size(1), parent(nullptr) {
        kids[0] = nullptr;
        kids[1] = nullptr;
    }

    int update_size() {
        if (kids[0] != nullptr) {
            size += kids[0]->update_size();
        }
        if (kids[1] != nullptr) {
            size += kids[1]->update_size();
        }
        return size;
    }
};

ostream& operator<<(ostream& out, const Node& node) {
    return out << "(" << node.index << ", " << node.value << ", " << node.size << ", " << node.parent->index << ")";
}

pair<vector<Node*>, vector<vector<int>>> setup(int N, const vector<int>& D) {
    cerrd << "Setting up...\n";

    vector<Node*> tree(N - 1);
    for (int i = 0; i < N - 1; ++i) {
        tree[i] = new Node();
        tree[i]->index = i;
        tree[i]->value = D[i];
    }

    vector<int> L(N - 1);
    vector<int> stack = {};
    for (int i = 0; i < N - 1; ++i) {
        while (!stack.empty() && D[stack.back()] < D[i]) {
            stack.pop_back();
        }
        L[i] = stack.empty() ? -1 : stack.back();
        stack.emplace_back(i);
    }

    cerrd << "Left = " << L << '\n';

    vector<int> R(N - 1);
    stack.clear();
    for (int i = N - 2; i >= 0; --i) {
        while (!stack.empty() && D[stack.back()] < D[i]) {
            stack.pop_back();
        }
        R[i] = stack.empty() ? -1 : stack.back();
        stack.emplace_back(i);
    }

    cerrd << "Right = " << R << '\n';

    Node* root = nullptr;
    for (int i = 0; i < N - 1; ++i) {
        if (L[i] == -1 && R[i] == -1) {
            root = tree[i];
        } else if (L[i] == -1) {
            tree[i]->parent = tree[R[i]];
            tree[R[i]]->kids[0] = tree[i];
        } else if (R[i] == -1) {
            tree[i]->parent = tree[L[i]];
            tree[L[i]]->kids[1] = tree[i];
        } else if (D[L[i]] < D[R[i]]) {
            tree[i]->parent = tree[L[i]];
            tree[L[i]]->kids[1] = tree[i];
        } else {
            tree[i]->parent = tree[R[i]];
            tree[R[i]]->kids[0] = tree[i];
        }
    }

    root->parent = root;
    root->update_size();

    cerrd << "Tree = " << tree << '\n';

    vector<vector<int>> A(tree.size(), vector<int>(BITS));
    for (int i = 0; i < N - 1; ++i) {
        A[i][0] = tree[i]->parent->index;
    }
    for (int j = 1; j < BITS; ++j) {
        for (int i = 0; i < N - 1; ++i) {
            A[i][j] = A[A[i][j - 1]][j - 1];
        }
    }

    // cerrd << "Ancestors = " << A << '\n';
    return {tree, A};
}

// int smartSolve(int N, int S, int K, const vector<int>& D) {
//     const auto [tree, A] = setup(N, D);
//     if (K == 1) {
//         for (Node* node : tree) delete node;
//         return S;
//     }

//     cerrd << "Checkpoint 1\n";

//     int first = (S > 1 && (S == N || D[S - 2] < D[S - 1])) ? (S - 2) : (S - 1);
//     if (tree[first]->size >= K - 1) {
//         if (first == S - 2) {
//             // Left
//             cerrd << "Checkpoint 2.A (Left)\n";
//             for (Node* node : tree) delete node;
//             return (S - (K - 1));
//         } else {
//             // Right
//             cerrd << "Checkpoint 2.B (Right)\n";
//             for (Node* node : tree) delete node;
//             return (S + (K - 1));
//         }
//     }

//     cerrd << "Checkpoint 2 (First = " << first << ").\n";

//     int node = first;
//     for (int i = BITS - 1; i >= 0; --i) {
//         if (tree[A[node][i]]->size < K - 1) {
//             node = A[node][i];
//         }
//     }
//     node = A[node][0];

//     cerrd << "Checkpoint 3 (Node = " << node << ")\n";

//     if (first < tree[node]->index) {
//         // Right
//         cerrd << "Checkpoint 3.A (Right)\n";
//         int room = (tree[node]->index + K - tree[node]->kids[0]->size);
//         for (Node* node : tree) delete node;
//         return room;
//     } else {
//         // Left
//         cerrd << "Checkpoint 3.B (Left)\n";
//         int room = (tree[node]->index + 3 - K + tree[node]->kids[1]->size);
//         for (Node* node : tree) delete node;
//         return room;
//     }
// }


// int bruteForce(int N, int S, int K, vector<int>& D) {
//     if (K == 1) {
//         return S;
//     }
//     int L = S - 2;
//     int R = S - 1;
//     while (--K) {
//         // cerrd << "BruteForce(" << L << ", " << R << ")\n";
//         if (L >= 0 && (R == N - 1 || D[L] < D[R])) {
//             // cerrd << "Chose L.\n";
//             if (K == 1) {
//                 return L + 1;
//             }
//             --L;
//         } else {
//             // cerrd << "Chose R.\n";
//             if (K == 1) {
//                 return R + 2;
//             }
//             ++R;
//         }
//     }
//     return -20;
// }

// void verify() {
//     auto rng = mt19937_64(random_device{}());
//     auto disN = uniform_int_distribution<int>(2, 10);
//     auto disQ = uniform_int_distribution<int>(1, 1);
//     for (int t = 0; t < 10; ++t) {
//         int N = disN(rng);
//         int Q = disQ(rng);
//         vector<int> D(N - 1);
//         iota(D.begin(), D.end(), 1);
//         shuffle(D.begin(), D.end(), rng);
//         for (int q = 0; q < Q; ++q) {
//             int S = uniform_int_distribution<int>{1, N}(rng);
//             int K = uniform_int_distribution<int>{1, N}(rng);
//             cerrd << "Trying with rooms " << D << ", N = " << N << ", S = " << S << ", and K = " << K << ".\n";
//             int want = bruteForce(N, S, K, D);
//             int have = smartSolve(N, S, K, D);
//             if (have != want) {
//                 cerrd << "Error: " << D << " " << S << " " << K << '\n';
//                 cerrd << "    Have = " << have << '\n';
//                 cerrd << "    Want = " << want << '\n';
//                 return;
//             }
//         }
//     }
// }

void solution() {
    // verify();
    // return;

    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        int N, Q;
        cin >> N >> Q;
        vector<int> D(N - 1);
        for (int i = 0; i < N - 1; ++i) {
            cin >> D[i];
        }

        const auto [tree, A] = setup(N, D);

        cout << "Case #" << t << ":";
        while (Q --> 0) {
            int S, K;
            cin >> S >> K;
            if (K == 1) {
                cout << ' ' << S;
                continue;
            }
            int first = (S > 1 && (S == N || D[S - 2] < D[S - 1])) ? (S - 2) : (S - 1);
            if (tree[first]->size >= K - 1) {
                if (first == S - 2) {
                    // Left
                    cout << ' ' << (S - (K - 1));
                } else {
                    // Right
                    cout << ' ' << (S + (K - 1));
                }
                continue;
            }

            int node = first;
            for (int i = BITS - 1; i >= 0; --i) {
                if (tree[A[node][i]]->size < K - 1) {
                    node = A[node][i];
                }
            }
            node = A[node][0];

            if (first < tree[node]->index) {
                // Right
                int room = (tree[node]->index + K - tree[node]->kids[0]->size);
                cout << ' ' << room;
            } else {
                // Left
                int room = (tree[node]->index + 3 - K + tree[node]->kids[1]->size);
                cout << ' ' << room;
            }
        }
        cout << '\n';

        for (Node* node : tree) {
            delete node;
        }
    }
}
