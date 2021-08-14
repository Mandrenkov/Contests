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

    BigInt operator<<(const BigInt&) const;
    BigInt& operator<<=(const BigInt&);

    BigInt operator>>(const BigInt&) const;
    BigInt& operator>>=(const BigInt&);

    bool operator==(const BigInt&) const;
    bool operator<(const BigInt&) const;

    bool operator!=(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    BigInt abs() const;
    BigInt pow(const BigInt&) const;

    std::string str() const;

    friend std::istream& operator>>(std::istream&, BigInt&);
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

    // Case 1: Sum the magnitudes of the LHS and RHS.
    // -------------------------------------------------------------------------
    if (lhs.positive == rhs.positive) {
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

    // Case 2: The LHS and RHS are negatives of each other.
    // -------------------------------------------------------------------------
    if (lhs.abs() == rhs.abs()) {
        lhs.positive = true;
        lhs.words = {0};
        return lhs;
    }

    // Case 3: Subtract the magnitudes of the LHS and RHS.
    // -------------------------------------------------------------------------

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

BigInt BigInt::operator*(const BigInt& rhs) const {
    BigInt lhs = *this;
    return lhs *= rhs;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
    BigInt& lhs = *this;

    lhs.positive = lhs.positive == rhs.positive || lhs == BigInt() || rhs == BigInt();

    // Take |large| >= |small| for convenience.
    const bool swapped = lhs.abs() < rhs.abs();
    const auto& large = swapped ? rhs : lhs;
    const auto& small = swapped ? lhs : rhs;

    // Base Case: The smaller operand fits into a single word.
    // -------------------------------------------------------------------------
    if (small.words.size() == 1) {
        uint64_t carry = 0;

        // Cache the small product operand (SPO) in case the loop overwrites it.
        const uint64_t spo = small.words[0];

        for (size_t i = 0; i < large.words.size() || carry > 0; ++i) {
            const uint64_t lpo = i < large.words.size() ? large.words[i] : 0;

            const uint64_t total = lpo * spo + carry;
            const uint64_t sum = total % base;
            carry = total / base;

            if (i == lhs.words.size()) {
                lhs.words.emplace_back(sum);
            } else {
                lhs.words[i] = sum;
            }
        }

        // Remove leading zeros.
        while (lhs.words.size() > 1 && lhs.words.back() == 0) {
            lhs.words.pop_back();
        }

        return lhs;
    }

    // Recursive Case: See https://en.wikipedia.org/wiki/Karatsuba_algorithm.
    // -------------------------------------------------------------------------
    const size_t shift = small.words.size() / 2;

    BigInt lhs_ [2] = {lhs, lhs};
    lhs_[0].words.erase(lhs_[0].words.begin() + shift, lhs_[0].words.end());
    lhs_[1].words.erase(lhs_[1].words.begin(), lhs_[1].words.begin() + shift);

    BigInt rhs_ [2] = {rhs, rhs};
    rhs_[0].words.erase(rhs_[0].words.begin() + shift, rhs_[0].words.end());
    rhs_[1].words.erase(rhs_[1].words.begin(), rhs_[1].words.begin() + shift);

    // Allocate a vector for prepending elements; padding a BigInt with n
    // zeros from the LSB side represents a multiplication by pow(base, n).
    const std::vector<word_t> buffer(2 * shift);

    BigInt z2 = lhs_[1] * rhs_[1];
    z2.words.insert(z2.words.begin(), buffer.begin(), buffer.end());

    BigInt z1 = (lhs_[1] * rhs_[0]) + (lhs_[0] * rhs_[1]);
    z1.words.insert(z1.words.begin(), buffer.begin(), buffer.begin() + shift);

    BigInt z0 = lhs_[0] * rhs_[0];

    lhs.words = (z2 + z1 + z0).words;
    return lhs;
}

BigInt BigInt::operator/(const BigInt& rhs) const {
    BigInt lhs = *this;
    lhs /= rhs;
    return lhs;
}

BigInt& BigInt::operator/=(const BigInt& rhs) {
    BigInt& lhs = *this;

    // Case 1: The divisor is zero.
    // -------------------------------------------------------------------------
    if (rhs == BigInt()) {
        throw "Divisor cannot be zero.";
    }

    // Case 2: The LHS and RHS both fit into a single word.
    // -------------------------------------------------------------------------
    if (lhs.words.size() == 1 && rhs.words.size() == 1) {
        lhs.positive = lhs.positive == rhs.positive;
        lhs.words = {lhs.words.front() / rhs.words.front()};
        return lhs;
    }

    // Case 3: The RHS exceeds the LHS.
    // -------------------------------------------------------------------------
    if (lhs.abs() < rhs.abs()) {
        lhs.positive = true;
        lhs.words = {0};
        return lhs;
    }

    // Case 4: The quotient must be computed using long division.
    // -------------------------------------------------------------------------
    BigInt top = lhs.abs();
    BigInt bot = rhs.abs();

    const bool positive = lhs.positive == rhs.positive;

    lhs.words = {0};

    while (top >= bot) {
        // Casting the word sizes to signed integers avoids underflow.
        const int top_num_words = top.words.size();
        const int bot_num_words = bot.words.size();

        // Compute the "ambitious" base exponent of the divisor.
        const size_t padding = std::max(0, top_num_words - bot_num_words);

        BigInt quotient;
        quotient.words.resize(padding);

        // Multiply the denominator by pow(base, padding).
        BigInt scaled_bot = bot;
        scaled_bot.words.insert(scaled_bot.words.begin(), quotient.words.begin(), quotient.words.end());

        if (top < scaled_bot) {
            // The divisor is too large; divide it by the base.
            const word_t scalar = top.words.back() * base / (bot.words.back() + 1);
            quotient.words.back() = scalar;
        } else {
            const word_t scalar = std::max<word_t>(1, top.words.back() / (bot.words.back() + 1));
            quotient.words.emplace_back(scalar);
        }

        lhs += quotient;
        top -= quotient * bot;
    }

    lhs.positive = positive;

    return lhs;
}

BigInt BigInt::operator%(const BigInt& rhs) const {
    BigInt lhs = *this;
    lhs %= rhs;
    return lhs;
}

BigInt& BigInt::operator%=(const BigInt& rhs) {
    BigInt& lhs = *this;
    return *this = (*this - *this / rhs * rhs);
}

BigInt BigInt::operator<<(const BigInt& rhs) const {
    return *this * BigInt(2).pow(rhs);
}

BigInt& BigInt::operator<<=(const BigInt& rhs) {
    return *this *= BigInt(2).pow(rhs);
}

BigInt BigInt::operator>>(const BigInt& rhs) const {
    return *this / BigInt(2).pow(rhs);
}

BigInt& BigInt::operator>>=(const BigInt& rhs) {
    return *this /= BigInt(2).pow(rhs);
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

BigInt BigInt::pow(const BigInt& exponent) const {
    if (!exponent.positive) {
        throw "Exponent cannot be negative.";
    } else if (exponent == BigInt()) {
        return BigInt(1);
    } else if (*this == BigInt()) {
        return *this;
    }

    BigInt power(1);
    power.positive = this->positive || (exponent.words.front() % 2 == 0);

    BigInt base = this->abs();

    for (BigInt exp = exponent; exp != BigInt(); exp /= 2) {
        if (exp.words.front() % 2 == 1) {
            power *= base;
        }
        base *= base;
    }

    return power;
}

std::string BigInt::str() const {
    std::stringstream out;
    out << *this;
    return out.str();
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

std::istream& operator>>(std::istream& in, BigInt& num) {
    std::string buffer;
    in >> buffer;
    num = BigInt(buffer);
    return in;
}
