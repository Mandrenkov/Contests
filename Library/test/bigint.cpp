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

        REQUIRE_THROWS(BigInt(""));
        REQUIRE_THROWS(BigInt("-"));
        REQUIRE_THROWS(BigInt("-123.0"));
        REQUIRE_THROWS(BigInt("-123abc456"));
    }
}

TEST_CASE("BigInt::operator-()", "[BigInt]") {
    CHECK(serialize(-BigInt()) == "0");
    CHECK(serialize(-BigInt(1)) == "-1");
    CHECK(serialize(-BigInt(-2)) == "2");
}

TEST_CASE("BigInt::operator==(...)", "[BigInt]") {
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

TEST_CASE("BigInt::operator<=>(...)", "[BigInt]") {
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

TEST_CASE("BigInt::operator+(...)", "[BigInt]") {
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

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis = std::uniform_int_distribution<long long>(std::numeric_limits<int>::min(),
                                                            std::numeric_limits<int>::max());

        for (size_t i = 0; i < 1000; ++i) {
            long long lhs = dis(rng);
            long long rhs = dis(rng);
            CHECK(BigInt(lhs + rhs) == BigInt(lhs) + BigInt(rhs));
        }
    }
}

TEST_CASE("BigInt::operator-(...)", "[BigInt]") {
    SECTION("(+) - (+)") {
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

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis = std::uniform_int_distribution<long long>(std::numeric_limits<int>::min(),
                                                            std::numeric_limits<int>::max());

        for (size_t i = 0; i < 1000; ++i) {
            long long lhs = dis(rng);
            long long rhs = dis(rng);
            CHECK(BigInt(lhs - rhs) == BigInt(lhs) - BigInt(rhs));
        }
    }
}

TEST_CASE("BigInt::operator*(...)", "[BigInt]") {
    SECTION("(+) * (+)") {
        CHECK(BigInt() * BigInt() == 0);
        CHECK(BigInt() * BigInt(1) == 0);
        CHECK(BigInt(1) * BigInt() == 0);

        CHECK(BigInt(1) * BigInt(2) == 2);
        CHECK(BigInt(2) * BigInt(1) == 2);

        CHECK(BigInt(2) * BigInt(3) == 6);
        CHECK(BigInt(12) * BigInt(11) == 132);
        CHECK(BigInt(123456) * BigInt(654321) == 80779853376LL);
        CHECK(BigInt(1234567890) * BigInt(987654321) == 1219326311126352690LL);
        CHECK(BigInt(660926254) * BigInt(56991325160) == std::string("37667063048494750640"));

        CHECK(BigInt("227665372209820782943832370508580399369381023172533882361878362635746171351356020645542347341639449") *
            BigInt("774505744878978419380135818762313719512453981555627919693266329339858782277905352210302880203552531") ==
            BigInt("176328138686517118613153971066150791079263727582641296951959147247738885835919148322410064339815297458079066748004125653796215057974347126019113518768116554387582588400916423826566715384951533395419"));
    }

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis = std::uniform_int_distribution<long long>(std::numeric_limits<int>::min(),
                                                            std::numeric_limits<int>::max());

        for (size_t i = 0; i < 1000; ++i) {
            unsigned long long lhs = dis(rng);
            unsigned long long rhs = dis(rng);
            CHECK(BigInt(lhs * rhs) == BigInt(lhs) * BigInt(rhs));
        }
    }
}


TEST_CASE("BigInt::abs()", "[BigInt]") {
    CHECK(serialize(BigInt().abs()) == "0");

    CHECK(serialize(BigInt(1).abs()) == "1");
    CHECK(serialize(BigInt(1234567890).abs()) == "1234567890");

    CHECK(serialize(BigInt(-1).abs()) == "1");
    CHECK(serialize(BigInt(-1234567890).abs()) == "1234567890");
}