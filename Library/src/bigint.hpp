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

    static void add(std::vector<word_t>&, const std::vector<word_t>&);
    static void sub(std::vector<word_t>&, const std::vector<word_t>&, bool);

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

BigInt BigInt::operator+(const BigInt& rhs) const {
    BigInt lhs = *this;
    lhs += rhs;
    return lhs;
}

BigInt& BigInt::operator+=(const BigInt& rhs) {
    BigInt& lhs = *this;

    if (lhs.positive == rhs.positive) {
        add(lhs.words, rhs.words);
        return lhs;
    }
    
    if (lhs.abs() == rhs.abs()) {
        lhs.positive = true;
        lhs.words = {0};
        return lhs;
    }

    if (rhs.abs() < lhs.abs()) {
        sub(lhs.words, rhs.words, false);
    } else {
        lhs.positive = rhs.positive;
        sub(lhs.words, rhs.words, true);
    }
    return lhs;
}

void BigInt::add(std::vector<word_t>& lhs, const std::vector<word_t>& rhs) {
    uint64_t carry = 0;

    for (size_t i = 0; i < lhs.size() || i < rhs.size() || carry > 0; ++i) {
        const uint64_t l = i < lhs.size() ? lhs[i] : 0;
        const uint64_t r = i < rhs.size() ? rhs[i] : 0;

        const uint64_t total = l + r + carry;
        const uint64_t sum = total % base;
        carry = total / base;

        if (i == lhs.size()) {
            lhs.emplace_back(sum);
        } else {
            lhs[i] = sum;
        }
    }
}

void BigInt::sub(std::vector<word_t>& lhs, const std::vector<word_t>& rhs, bool swapped) {
    // Always mutate the original LHS argument but take |lhs_| >= |rhs_| for convenience.
    std::vector<word_t>& ans = lhs;
    const std::vector<word_t>& lhs_ = swapped ? rhs : lhs;
    const std::vector<word_t>& rhs_ = swapped ? lhs : rhs;

    // Tracks the quantity of the next |lhs_| word that has been "borrowed".
    uint64_t debt = 0;

    for (size_t i = 0; i < lhs_.size() || i < rhs_.size(); ++i) {
        const uint64_t l = i < lhs_.size() ? lhs_[i] : 0;
        const uint64_t r = i < rhs_.size() ? rhs_[i] : 0;

        uint64_t diff;
        if (l >= r + debt) {
            diff = l - r - debt;
            debt = 0;
        } else {
            // Borrow from the next word (even if it must be done transitively).
            diff = l + base - r - debt;
            debt = 1;
        }

        // Since |lhs_| >= |rhs_| there is never a need to extend |ans|.
        ans[i] = diff;
    }

    // Remove leading zeros.
    while (ans.size() > 1 && ans.back() == 0) {
        ans.pop_back();
    }
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
