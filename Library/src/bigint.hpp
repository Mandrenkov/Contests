#include <bits/stdc++.h>

class BigInt {
public:
    BigInt();
    BigInt(int64_t);
    BigInt(const std::string&);

    BigInt operator-() const;

    BigInt operator+(const BigInt&) const;
    BigInt& operator+=(const BigInt&);

    BigInt operator-(const BigInt&) const;
    BigInt& operator-=(const BigInt&);

    BigInt operator*(const BigInt&) const;
    BigInt& operator*=(const BigInt&);

    BigInt operator/(const BigInt&) const;
    BigInt& operator/=(const BigInt&);

    BigInt operator%(const BigInt&) const;
    BigInt& operator%=(const BigInt&);

    bool operator==(const BigInt&) const;
    bool operator<(const BigInt&) const;

    bool operator!=(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    BigInt abs() const;

    friend std::ostream& operator<<(std::ostream&, const BigInt&);

private:
    using word_t = uint32_t;

    // Making the base a power of 10 is useful for output streaming.
    static constexpr uint64_t base = 1000 * 1000 * 1000;

    bool positive;
    std::vector<word_t> words;
};


BigInt::BigInt() : BigInt(0) {}

BigInt::BigInt(int64_t val) {
    if (val == 0) {
        positive = true;
        words = {0};
        return;
    }

    uint64_t mag;
    if (val < 0) {
        positive = false;
        mag = static_cast<uint64_t>(-(val + 1)) + 1;
    } else {
        positive = true;
        mag = static_cast<uint64_t>(val);
    }

    while (mag > 0) {
        words.emplace_back(mag % base);
        mag /= base;
    }
}

BigInt::BigInt(const std::string& val) {
    static const std::string error =
        "Argument to BigInt(const std::string&) does not match (-?[0-9]+).";

    if (val == "" || (val == "-")) {
        throw error;
    }

    // To be corrected if the remainder of the input is zero.
    positive = val.front() != '-';

    // Take the first and last index of the magnitude.
    const int beg = positive ? 0 : 1;
    const int end = val.size() - 1;

    static const int chunk = std::log10(base);

    for (int i = end; i >= beg; i -= chunk) {
        word_t word = 0;
        for (int j = std::max(beg, i - chunk + 1); j <= i; ++j) {
            if (val[j] < '0' || val[j] > '9') {
                throw error;
            }
            word = 10 * word + val[j] - '0';
        }
        words.emplace_back(word);
    }

    // Remove leading zeros.
    while (words.size() > 1 && words.back() == 0) {
        words.pop_back();
    }

    // Correct the sign in case the input is zero.
    positive = positive || words == std::vector<word_t>{0};
}

BigInt BigInt::operator-() const {
    BigInt num = *this;
    if (num != BigInt()) {
        num.positive = !positive;
    }
    return num;
}

BigInt BigInt::operator+(const BigInt& rhs) const {
    BigInt lhs = *this;
    return lhs += rhs;
}

BigInt& BigInt::operator+=(const BigInt& rhs) {
    BigInt& lhs = *this;

    if (lhs.positive == rhs.positive) {
        // Sum the magnitudes of the LHS and RHS.
        uint64_t carry = 0;

        for (size_t i = 0; i < lhs.words.size() || i < rhs.words.size() || carry > 0; ++i) {
            const uint64_t l = i < lhs.words.size() ? lhs.words[i] : 0;
            const uint64_t r = i < rhs.words.size() ? rhs.words[i] : 0;

            const uint64_t total = l + r + carry;
            const uint64_t sum = total % base;
            carry = total / base;

            if (i == lhs.words.size()) {
                lhs.words.emplace_back(sum);
            } else {
                lhs.words[i] = sum;
            }
        }

        return lhs;
    }

    if (lhs.abs() == rhs.abs()) {
        // The sum is exactly 0.
        lhs.positive = true;
        lhs.words = {0};
        return lhs;
    }

    const bool sign_swap = lhs.abs() < rhs.abs();

    if (sign_swap) {
        lhs.positive = rhs.positive;
    }

    // Take |large| >= |small| for convenience.
    const auto& large = sign_swap ? rhs : lhs;
    const auto& small = sign_swap ? lhs : rhs;

    // Track the quantity of the next word from |large| that has been "borrowed".
    uint64_t debt = 0;

    for (size_t i = 0; i < large.words.size(); ++i) {
        const uint64_t l = i < large.words.size() ? large.words[i] : 0;
        const uint64_t s = i < small.words.size() ? small.words[i] : 0;

        uint64_t diff;
        if (l >= s + debt) {
            diff = l - s - debt;
            debt = 0;
        } else {
            // Borrow from the next word (even if it must be done transitively).
            diff = l + base - s - debt;
            debt = 1;
        }

        if (i == lhs.words.size()) {
            lhs.words.emplace_back(diff);
        } else {
            lhs.words[i] = diff;
        }
    }

    // Remove leading zeros.
    while (lhs.words.size() > 1 && lhs.words.back() == 0) {
        lhs.words.pop_back();
    }

    return lhs;
}

BigInt BigInt::operator-(const BigInt& rhs) const {
    return *this + (-rhs);
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
    return *this += -rhs;
}

bool BigInt::operator==(const BigInt& rhs) const {
    return positive == rhs.positive && words == rhs.words;
}

bool BigInt::operator<(const BigInt& rhs) const {
    const BigInt& lhs = *this;

    if (lhs.positive ^ rhs.positive) {
        return rhs.positive;
    }

    // Ensure that (lhs < rhs) && (rhs < lhs) never happens.
    if (lhs.words == rhs.words) {
        return false;
    }

    // Account for the lttle-endian ordering of words.
    if (lhs.words.size() != rhs.words.size()) {
        return lhs.words.size() < rhs.words.size() == lhs.positive;
    }

    const bool comparison = std::lexicographical_compare(lhs.words.rbegin(), lhs.words.rend(),
                                                         rhs.words.rbegin(), rhs.words.rend());
    return lhs.positive ? comparison : !comparison;
}

bool BigInt::operator!=(const BigInt& rhs) const {
    return !(*this == rhs);
}

bool BigInt::operator<=(const BigInt& rhs) const {
    return !(rhs < *this);
}

bool BigInt::operator>(const BigInt& rhs) const {
    return rhs < *this;
}

bool BigInt::operator>=(const BigInt& rhs) const {
    return !(*this < rhs);
}

BigInt BigInt::abs() const {
    BigInt num = *this;
    num.positive = true;
    return num;
}

std::ostream& operator<<(std::ostream& out, const BigInt& num) {
    if (!num.positive) {
        out << '-';
    }

    std::ostream init(NULL);
    init.copyfmt(out);

    for (auto it = num.words.rbegin(); it != num.words.rend(); ++it) {
        if (it != num.words.rbegin()) {
            out.fill('0');
            out.width(9);
        }
        out << *it;
    }

    out.copyfmt(init);

    return out;
}
