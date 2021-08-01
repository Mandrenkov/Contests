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
        CHECK(serialize(BigInt(1)) == "1");
        CHECK(serialize(BigInt(123)) == "123");
        CHECK(serialize(BigInt(999999999)) == "999999999");
        CHECK(serialize(BigInt(1000000000)) == "1000000000");
        CHECK(serialize(BigInt(9223372036854775807LL)) == "9223372036854775807");

        CHECK(serialize(BigInt(-0)) == "0");
        CHECK(serialize(BigInt(-1)) == "-1");
        CHECK(serialize(BigInt(-999999999)) == "-999999999");
        CHECK(serialize(BigInt(-1000000000)) == "-1000000000");
        CHECK(serialize(BigInt(-9223372036854775807LL)) == "-9223372036854775807");
    }

    SECTION("BigInt(const std::string&)") {
        CHECK(serialize(BigInt("0")) == "0");
        CHECK(serialize(BigInt("123")) == "123");
        CHECK(serialize(BigInt("999999999")) == "999999999");
        CHECK(serialize(BigInt("01000000000")) == "1000000000");
        CHECK(serialize(BigInt("9223372036854775807")) == "9223372036854775807");
        CHECK(serialize(BigInt("00000000000000000000000000")) == "0");

        CHECK(serialize(BigInt("-0")) == "0");
        CHECK(serialize(BigInt("-1")) == "-1");
        CHECK(serialize(BigInt("-999999999")) == "-999999999");
        CHECK(serialize(BigInt("-01000000000")) == "-1000000000");
        CHECK(serialize(BigInt("-9223372036854775807")) == "-9223372036854775807");
        CHECK(serialize(BigInt("-00000000000000000000000000")) == "0");
    }
}

TEST_CASE("BigInt::operator-()", "[BigInt]") {
    CHECK(serialize(-BigInt()) == "0");
    CHECK(serialize(-BigInt(1)) == "-1");
    CHECK(serialize(-BigInt(-2)) == "2");
}

TEST_CASE("BigInt::operator==()", "[BigInt]") {
    SECTION("operator==") {
        CHECK(BigInt() == BigInt());
        CHECK(BigInt(123) == BigInt(123));
        CHECK(BigInt(-1234567890) == BigInt(-1234567890));
    }

    SECTION("operator!=") {
        CHECK(BigInt() != BigInt(1));
        CHECK(BigInt(123) != BigInt(321));
        CHECK(BigInt(1234567890) != BigInt(-1234567890));
        CHECK(BigInt(1234567890) != BigInt(1234567891));
    }
}

TEST_CASE("BigInt::operator<=>()", "[BigInt]") {
    SECTION("operator<") {
        CHECK(BigInt(-2) < BigInt(-1));
        CHECK(BigInt(-1) < BigInt(0));
        CHECK(BigInt(0) < BigInt(1));
        CHECK(BigInt(1) < BigInt(2));

        CHECK(BigInt(-1234567891) < BigInt(-1234567890));
        CHECK(BigInt(1234567890) < BigInt(1234567891));
    }

    SECTION("operator>") {
        CHECK(BigInt(2) > BigInt(1));
        CHECK(BigInt(1) > BigInt(0));
        CHECK(BigInt(0) > BigInt(-1));
        CHECK(BigInt(-1) > BigInt(-2));
    }

    SECTION("operator<=") {
        CHECK(BigInt(-2) <= BigInt(-2));
        CHECK(BigInt(-2) <= BigInt(-1));
        CHECK(BigInt(-2) <= BigInt(0));
        CHECK(BigInt(-2) <= BigInt(1));
    }

    SECTION("operator>=") {
        CHECK(BigInt(2) >= BigInt(2));
        CHECK(BigInt(2) >= BigInt(1));
        CHECK(BigInt(2) >= BigInt(0));
        CHECK(BigInt(2) >= BigInt(-1));
    }
}

TEST_CASE("BigInt::operator+()", "[BigInt]") {
    SECTION("(+) + (+) or (-) + (-)") {
        CHECK(BigInt() + BigInt() == 0);

        CHECK(BigInt(500000000) + BigInt(500000000) == 1000000000);
        CHECK(BigInt(-500000000) + BigInt(-500000000) == -1000000000);

        CHECK(BigInt(1) + BigInt(2) == 3);
        CHECK(BigInt(-2) + BigInt(-1) == -3);

        CHECK(BigInt(999999999) + BigInt(1) == 1000000000);
        CHECK(BigInt(-1) + BigInt(-999999999) == -1000000000);

        CHECK(BigInt(1234567890) + BigInt(1234567890) == 2469135780);
        CHECK(BigInt(-1234567890) + BigInt(-1234567890) == -2469135780);

        CHECK(BigInt("753933910438083927237676319556802292824649773437835919048001965958225342647424502670448395286767748") +
              BigInt("889120464295620859874489506906216159541811927511006472679060203691340689788016525584254564540511369") ==
              BigInt("1643054374733704787112165826463018452366461700948842391727062169649566032435441028254702959827279117"));
    }

    SECTION("(+) + (-) or (-) + (+)") {
        CHECK(BigInt(123) + BigInt(-123) == 0);
        CHECK(BigInt(-123) + BigInt(123) == 0);

        CHECK(BigInt(654321) + BigInt(-123456) == 530865);
        CHECK(BigInt(-654321) + BigInt(123456) == -530865);

        CHECK(BigInt(1000000000) + BigInt(-1) == 999999999);
        CHECK(BigInt(-1000000000) + BigInt(1) == -999999999);

        CHECK(BigInt(1234567890) + BigInt(-987654321) == 246913569);
        CHECK(BigInt(-1234567890) + BigInt(987654321) == -246913569);

        CHECK(BigInt(1000000000LL * 1000000000LL) + BigInt(-1) == 1000000000LL * 1000000000LL - 1LL);
        CHECK(BigInt(-1000000000LL * 1000000000LL) + BigInt(1) == -1000000000LL * 1000000000LL + 1LL);

        CHECK(BigInt("753933910438083927237676319556802292824649773437835919048001965958225342647424502670448395286767748") +
              BigInt("-889120464295620859874489506906216159541811927511006472679060203691340689788016525584254564540511369") ==
              BigInt("-135186553857536932636813187349413866717162154073170553631058237733115347140592022913806169253743621"));
    }
}

TEST_CASE("BigInt::operator-(...)", "[BigInt]") {
    CHECK(BigInt() - BigInt() == 0);
    CHECK(BigInt(1234567890) - BigInt(1234567890) == 0);

    CHECK(BigInt(3) - BigInt(2) == 1);
    CHECK(BigInt(2) - BigInt(3) == -1);

    CHECK(BigInt(1234567890) - BigInt(987654321) == 246913569);
    CHECK(BigInt(987654321) - BigInt(1234567890) == -246913569);

    CHECK(BigInt(1000000000LL * 1000000000LL) - BigInt(1) == 1000000000LL * 1000000000LL - 1LL);
    CHECK(BigInt(1) - BigInt(1000000000LL * 1000000000LL) == -1000000000LL * 1000000000LL + 1LL);

    CHECK(BigInt("753933910438083927237676319556802292824649773437835919048001965958225342647424502670448395286767748") -
          BigInt("889120464295620859874489506906216159541811927511006472679060203691340689788016525584254564540511369") ==
          BigInt("-135186553857536932636813187349413866717162154073170553631058237733115347140592022913806169253743621"));
}


TEST_CASE("BigInt::abs()", "[BigInt]") {
    CHECK(serialize(BigInt().abs()) == "0");

    CHECK(serialize(BigInt(1).abs()) == "1");
    CHECK(serialize(BigInt(1234567890).abs()) == "1234567890");

    CHECK(serialize(BigInt(-1).abs()) == "1");
    CHECK(serialize(BigInt(-1234567890).abs()) == "1234567890");
}