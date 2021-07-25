#include <bits/stdc++.h>

class BigInt {
public:
    BigInt();
    BigInt(int64_t);
    BigInt(const std::string&);

    BigInt operator+(const BigInt&) const;
    BigInt& operator+=(const Bigint&);

    BigInt operator-(const BigInt&) const;
    BigInt& operator-=(const Bigint&);

    BigInt operator*(const BigInt&) const;
    BigInt& operator*=(const Bigint&);

    BigInt operator/(const BigInt&) const;
    BigInt& operator/=(const Bigint&);

    BigInt operator%(const BigInt&) const;
    BigInt& operator%=(const Bigint&);

    BigInt& operator-=(const Bigint&);

    friend std::ostream& operator<<(std::ostream&, const BigInt&);

private:
    using word_t = uint32_t;

    static constexpr uint64_t base = numeric_limits<word_t>::max();

    bool sign;
    vector<word_t> words;
};


BigInt::BigInt() : BigInt(0) {}

BigInt::BigInt(int64_t val) {
    if (val == 0) {
        sign = true;
        data = {0};
        return;
    }

    uint64_t mag;
    if (val < 0) {
        sign = false;
        mag = static_cast<uint64_t>(-(val + 1)) + 1;
    } else {
        sign = true;
        mag = val;
    }

    while (mag > 0) {
        data.emplace_back(mag % base);
        mag /= base;
    }
}

BigInt operator+(const BigInt& rhs) const {
    BigInt sum;
    sum += rhs;
    return sum;
}