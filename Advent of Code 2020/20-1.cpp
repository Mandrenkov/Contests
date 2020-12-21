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

template<
    // Output stream overload for STL containers with one explicit type parameter.
    template<class> class Range, class T,
    typename = enable_if_t<is_same_v<Range<T>, deque<T>> ||
                           is_same_v<Range<T>, forward_list<T>> ||
                           is_same_v<Range<T>, list<T>> ||
                           is_same_v<Range<T>, multiset<T>> ||
                           is_same_v<Range<T>, set<T>> ||
                           is_same_v<Range<T>, unordered_multiset<T>> ||
                           is_same_v<Range<T>, unordered_set<T>> ||
                           is_same_v<Range<T>, vector<T>>>
>
ostream& operator<<(ostream& out, const Range<T>& range) {
    constexpr auto brackets = is_same_v<Range<T>, vector<T>> ? "[]" : "{}";
    out << brackets[0];
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it << (next(it) != range.end() ? ", " : "");
    }
    return out << brackets[1];
}

template<
    // Output stream overload for STL containers with two explicit type parameters.
    template<class, class> class Range, class K, class V,
    typename = enable_if_t<is_same_v<Range<K, V>, map<K, V>> ||
                           is_same_v<Range<K, V>, multimap<K, V>> ||
                           is_same_v<Range<K, V>, unordered_map<K, V>> ||
                           is_same_v<Range<K, V>, unordered_multimap<K, V>>>
>
ostream& operator<<(ostream& out, const Range<K, V>& range) {
    out << '{';
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << it->first << ": " << it->second << (next(it) != range.end() ? ", " : "");
    }
    return out << '}';
}

template <class F, class S>
ostream& operator<<(ostream& out, const pair<F, S>& duo) {
  return out << '(' << duo.first << ", " << duo.second << ')';
}

// Hash Template Specializations
template<class T, class U>
struct hash<std::pair<T, U>> {
    size_t operator()(const std::pair<T, U>& pair) const noexcept {
        return std::hash<T>{}(pair.first) ^ (std::hash<U>{}(pair.second) << 1);
    }
};

void solution();

// Initalize the execution environment and call the solution function.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    solution();
}

// -----------------------------------------------------------------------------

constexpr int MAX_ATTEMPTS = 100000;

struct Tile {
    int id;
    // The ordering of |sides| is [N, E, S, W].
    // ^--->
    // |   |
    // <---v
    vector<vector<bool>> sides;

    // Rotates this Tile 90 degrees clockwise.
    Tile rotate90(int turns) const {
        return Tile {
            id, {sides[(4 - turns) % 4],
                 sides[(5 - turns) % 4],
                 sides[(6 - turns) % 4],
                 sides[(7 - turns) % 4]}
        };
    }

    // Flips this Tile horizontally.
    Tile flip() const {
        auto sides = this->sides;
        for (auto& side : sides) {
            reverse(side.begin(), side.end());
        }
        swap(sides[1], sides[3]);
        return Tile{id, sides};
    }

    // Reports whether this Tile can go above the given Tile.
    bool above(const Tile& other) const {
        int length = sides[2].size();
        for (int i = 0; i < length; ++i) {
            if (sides[2][i] != other.sides[0][length - 1 - i]) {
                return false;
            }
        }
        return true;
    }

    // Reports whether this Tile can go to the left of the given Tile.
    bool before(const Tile& other) const {
        int length = sides[1].size();
        for (int i = 0; i < length; ++i) {
            if (sides[1][i] != other.sides[3][length - 1 - i]) {
                return false;
            }
        }
        return true;
    }
};

ostream& operator<<(ostream& out, const Tile& tile) {
    int length = tile.sides.front().size();
    vector2D<char> img(length, vector<char>(length, ' '));
    for (int i = 0; i < length; ++i) {
        img[0][i] = tile.sides[0][i] ? '#' : '.';
        img[i][length - 1] = tile.sides[1][i] ? '#' : '.';
        img[length - 1][length - 1 - i] = tile.sides[2][i] ? '#' : '.';
        img[length - 1 - i][0] = tile.sides[3][i] ? '#' : '.';
    }
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
            out << img[i][j];
        }
        out << '\n';
    }
    return out;
}

// Try to piece together the image array in a row-wise manner from left-to-right using a backtracking DFS.
bool arrange(const int length, const vector<Tile>& tiles, vector<vector<int>>& image, vector<bool>& candidates, int& attempts, int row, int col) {
    if (row == length) {
        return true;
    } else if (col == length) {
        return arrange(length, tiles, image, candidates, ++attempts, row + 1, 0);
    } else if (attempts >= MAX_ATTEMPTS) {
        return false;
    }

    for (int i = 0; i < candidates.size(); ++i) {
        if (!candidates[i]) {
            continue;
        }
        for (int j = 0; j < 12; ++j) {
            int index = 12 * i + j;
            const Tile& tile = tiles[index];
            if (row > 0) {
                const Tile& up = tiles[image[row - 1][col]];
                if (!up.above(tile)) {
                    continue;
                }
            }
            if (col > 0) {
                const Tile& left = tiles[image[row][col - 1]];
                if (!left.before(tile)) {
                    continue;
                }
            }
            candidates[i] = false;
            image[row][col] = index;
            if (arrange(length, tiles, image, candidates, ++attempts, row, col + 1)) {
                return true;
            }
            candidates[i] = true;
        }
    }
    return false;
}

void solution() {
    int N, S;
    cin >> N >> S;
    vector<Tile> tiles;
    for (int i = 0; i < N; ++i) {
        string word;
        cin >> word >> word;
        int id = stoi(word.substr(0, word.size() - 1));
        vector<string> image(S);
        for (int i = 0; i < S; ++i) {
            cin >> image[i];
        }
        vector2D<bool> sides(4, vector<bool>(S));
        for (int i = 0; i < S; ++i) {
            sides[0][i] = image[0][i] == '#';
            sides[2][i] = image[S - 1][S - 1 - i] == '#';
            sides[1][i] = image[i][S - 1] == '#';
            sides[3][i] = image[S - 1 - i][0] == '#';
        }
        Tile base{id, sides};
        Tile flip_h = base.flip();
        Tile flip_v = base.rotate90(1).flip().rotate90(3);
        for (const Tile& tile : {base, flip_h, flip_v}) {
            for (int turns = 0; turns < 4; ++turns) {
                tiles.emplace_back(tile.rotate90(turns));
            }
        }
    }

    // Compute the number of tiles along a dimension of the camera array.
    const int length = static_cast<int>(round(sqrt(N)));

    int attempts;
    vector2D<int> image;
    vector<bool> candidates;

    mt19937 rng(random_device{}());

    do {
        // Shuffle the tiles into a random permutation.
        vector<int> order(N);
        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), rng);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < 12; ++j) {
                swap(tiles[i * 12 + j], tiles[order[i] * 12 + j]);
            }
        }

        // Reset the attempts counter, image, and candidates after the last
        // arrangement triggered too many "attempts".
        attempts = 0;
        image = vector2D<int>(length, vector<int>(length));
        candidates = vector<bool>(N, true);
    } while (!arrange(length, tiles, image, candidates, attempts, 0, 0));

    long long corners = 1;
    corners *= tiles[image.front().front()].id;
    corners *= tiles[image.front().back()].id;
    corners *= tiles[image.back().front()].id;
    corners *= tiles[image.back().back()].id;
    cout << corners << '\n';
}
