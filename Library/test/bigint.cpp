#include <bits/stdc++.h>

#include <catch2/catch.hpp>

#include "bigint.hpp"

std::string serialize(const BigInt& num) {
    std::stringstream out;
    out << num;
    return out.str();
};

TEST_CASE("BigInt::BigInt(...)", "[BigInt]") {
    SECTION("BigInt()") {
        CHECK(serialize(BigInt()) == "0");
    }

    SECTION("BigInt(uint64_t)") {
        CHECK(serialize(BigInt(1LL)) == "1");
        CHECK(serialize(BigInt(123LL)) == "123");
        CHECK(serialize(BigInt(999999999LL)) == "999999999");
        CHECK(serialize(BigInt(1000000000LL)) == "1000000000");
        CHECK(serialize(BigInt(9223372036854775807LL)) == "9223372036854775807");

        CHECK(serialize(BigInt(-0LL)) == "0");
        CHECK(serialize(BigInt(-1LL)) == "-1");
        CHECK(serialize(BigInt(-999999999LL)) == "-999999999");
        CHECK(serialize(BigInt(-1000000000LL)) == "-1000000000");
        CHECK(serialize(BigInt(-9223372036854775807LL)) == "-9223372036854775807");
    }
}

TEST_CASE("BigInt::abs()", "[BigInt]") {
    CHECK(serialize(BigInt().abs()) == "0");

    CHECK(serialize(BigInt(1LL).abs()) == "1");
    CHECK(serialize(BigInt(987654321000LL).abs()) == "987654321000");

    CHECK(serialize(BigInt(-1LL).abs()) == "1");
    CHECK(serialize(BigInt(-987654321000LL).abs()) == "987654321000");
}