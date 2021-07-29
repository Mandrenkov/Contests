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

BigInt BigInt::operator-() const {
    BigInt num = *this;
    if (num != BigInt()) {
        num.positive = !positive;
    }
    return num;
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
