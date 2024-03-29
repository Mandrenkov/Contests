#include <iostream>
#include <sstream>
#include <string>

#include <catch2/catch.hpp>

#include "bigint.hpp"

constexpr int NUM_RANDOM_TESTS = 100;

TEST_CASE("BigInt::BigInt", "[BigInt]") {
    SECTION("BigInt()") {
        CHECK(BigInt().str() == "0");
    }

    SECTION("BigInt(int64_t)") {
        CHECK(BigInt(1).str() == "1");
        CHECK(BigInt(123).str() == "123");
        CHECK(BigInt(999999999).str() == "999999999");
        CHECK(BigInt(1000000000).str() == "1000000000");
        CHECK(BigInt(9223372036854775807LL).str() == "9223372036854775807");

        CHECK(BigInt(-0).str() == "0");
        CHECK(BigInt(-1).str() == "-1");
        CHECK(BigInt(-999999999).str() == "-999999999");
        CHECK(BigInt(-1000000000).str() == "-1000000000");
        CHECK(BigInt(-9223372036854775807LL).str() == "-9223372036854775807");
    }

    SECTION("BigInt(const std::string&)") {
        CHECK(BigInt("0").str() == "0");
        CHECK(BigInt("123").str() == "123");
        CHECK(BigInt("999999999").str() == "999999999");
        CHECK(BigInt("01000000000").str() == "1000000000");
        CHECK(BigInt("9223372036854775807").str() == "9223372036854775807");
        CHECK(BigInt("00000000000000000000000000").str() == "0");

        CHECK(BigInt("-0").str() == "0");
        CHECK(BigInt("-1").str() == "-1");
        CHECK(BigInt("-999999999").str() == "-999999999");
        CHECK(BigInt("-01000000000").str() == "-1000000000");
        CHECK(BigInt("-9223372036854775807").str() == "-9223372036854775807");
        CHECK(BigInt("-00000000000000000000000000").str() == "0");

        REQUIRE_THROWS(BigInt(""));
        REQUIRE_THROWS(BigInt("-"));
        REQUIRE_THROWS(BigInt("-123.0"));
        REQUIRE_THROWS(BigInt("-123abc456"));
    }
}

TEST_CASE("istream::operator>>", "[BigInt]") {
    const auto deserialize = [](const std::string& input) {
        BigInt num;
        std::stringstream in(input);
        in >> num;
        return num;
    };

    CHECK(deserialize("0") == 0);
    CHECK(deserialize("1") == 1);
    CHECK(deserialize("-1") == -1);

    CHECK(deserialize("123456") == 123456);
    CHECK(deserialize("1234567890") == 1234567890);
    CHECK(deserialize("-12345678987654321") == -12345678987654321LL);

    REQUIRE_THROWS(deserialize(""));
    REQUIRE_THROWS(deserialize("-"));
    REQUIRE_THROWS(deserialize("-123.0"));
    REQUIRE_THROWS(deserialize("-123abc456"));
}

TEST_CASE("BigInt::operator-()", "[BigInt]") {
    CHECK(-BigInt() == BigInt("0"));
    CHECK(-BigInt(1) == BigInt("-1"));
    CHECK(-BigInt(-2) == BigInt("2"));
}

TEST_CASE("BigInt::operator==", "[BigInt]") {
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

TEST_CASE("BigInt::operator<=>", "[BigInt]") {
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

TEST_CASE("BigInt::operator+", "[BigInt]") {
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
        auto dis = std::uniform_int_distribution<int>(std::numeric_limits<int>::min(),
                                                      std::numeric_limits<int>::max());

        for (size_t i = 0; i < NUM_RANDOM_TESTS; ++i) {
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
        auto dis = std::uniform_int_distribution<int>(std::numeric_limits<int>::min(),
                                                      std::numeric_limits<int>::max());

        for (size_t i = 0; i < NUM_RANDOM_TESTS; ++i) {
            long long lhs = dis(rng);
            long long rhs = dis(rng);
            CHECK(BigInt(lhs - rhs) == BigInt(lhs) - BigInt(rhs));
        }
    }
}

TEST_CASE("BigInt::operator*", "[BigInt]") {
    SECTION("(+) * (+)") {
        CHECK(BigInt() * BigInt() == 0);
        CHECK(BigInt() * BigInt(1) == 0);
        CHECK(BigInt(1) * BigInt() == 0);

        CHECK(BigInt() * BigInt(1234567890) == 0);
        CHECK(BigInt(1234567890) * BigInt() == 0);

        CHECK(BigInt(1) * BigInt(2) == 2);
        CHECK(BigInt(2) * BigInt(1) == 2);

        CHECK(BigInt(2) * BigInt(3) == 6);
        CHECK(BigInt(12) * BigInt(11) == 132);
        CHECK(BigInt(123456) * BigInt(654321) == 80779853376LL);
        CHECK(BigInt(1234567890) * BigInt(987654321) == 1219326311126352690LL);
        CHECK(BigInt(660926254) * BigInt(56991325160) == BigInt("37667063048494750640"));

        CHECK(BigInt("227665372209820782943832370508580399369381023172533882361878362635746171351356020645542347341639449") *
              BigInt("774505744878978419380135818762313719512453981555627919693266329339858782277905352210302880203552531") ==
              BigInt("176328138686517118613153971066150791079263727582641296951959147247738885835919148322410064339815297458079066748004125653796215057974347126019113518768116554387582588400916423826566715384951533395419"));
    }

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis = std::uniform_int_distribution<long long>(std::numeric_limits<int>::min(),
                                                            std::numeric_limits<int>::max());

        for (size_t i = 0; i < NUM_RANDOM_TESTS; ++i) {
            long long lhs = dis(rng);
            long long rhs = dis(rng);
            CHECK(BigInt(lhs * rhs) == BigInt(lhs) * BigInt(rhs));
        }
    }
}

TEST_CASE("BigInt::operator/", "[BigInt]") {
    SECTION("(+) / (+)") {
        CHECK(BigInt(2) / BigInt(1) == 2);
        CHECK(BigInt(3) / BigInt(1) == 3);

        CHECK(BigInt(12) / BigInt(3) == 4);
        CHECK(BigInt(13) / BigInt(3) == 4);
        CHECK(BigInt(14) / BigInt(3) == 4);
        CHECK(BigInt(15) / BigInt(3) == 5);

        CHECK(BigInt(12345678987654321LL) / BigInt(567898765) == 21739224);

        CHECK(BigInt("58847343977952139032542966407431703812923494687402576708815231538311544334737455357296781995384767204567613931092320091731010684255920640047749087714424580992008002606070216209999266295000523751033898") /
              BigInt("2219439555864229884262149556907332886240547352715782147178872142509690145124314872388696800954833420") ==
              BigInt("26514506251123162650792152172252253366139553102242710020362983347531680475643178600747376361764305970"));

        CHECK(BigInt("9832669330483809071367926297102810309779485703236475414705978560881507106897177670904211402264602446") /
              BigInt("75557863725914323419136") ==
              BigInt("130134295037135451221794315165446946109466472663220581233866604309085671619914"));
    }

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis = std::uniform_int_distribution<long long>(std::numeric_limits<long long>::min(),
                                                            std::numeric_limits<long long>::max());

        for (size_t i = 0; i < NUM_RANDOM_TESTS; ++i) {
            long long lhs = dis(rng);
            long long rhs = dis(rng);
            CHECK(BigInt(lhs / rhs) == BigInt(lhs) / BigInt(rhs));
        }
    }
}

TEST_CASE("BigInt::operator%", "[BigInt]") {
    SECTION("(+) % (+)") {
        CHECK(BigInt(0) % BigInt(1) == 0);
        CHECK(BigInt(1) % BigInt(1) == 0);
        CHECK(BigInt(2) % BigInt(1) == 0);
        CHECK(BigInt(3) % BigInt(1) == 0);

        CHECK(BigInt(12) % BigInt(3) == 0);
        CHECK(BigInt(13) % BigInt(3) == 1);
        CHECK(BigInt(14) % BigInt(3) == 2);
        CHECK(BigInt(15) % BigInt(3) == 0);

        CHECK(BigInt(12345678987654321LL) % BigInt(567898765) == 525995961);

        CHECK(BigInt("58847343977952139032542966407431703812923494687402576708815231538311544334737455357296781995384767204567613931092320091731010684255920640047749087714424580992008002606070216209999266295000523751033898") %
              BigInt("2219439555864229884262149556907332886240547352715782147178872142509690145124314872388696800954833420") ==
              BigInt("1045485520912884296755561114933801153761692013833341320740745009339263452144753414004535020489516498"));
    }

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis = std::uniform_int_distribution<long long>(std::numeric_limits<long long>::min(),
                                                            std::numeric_limits<long long>::max());

        for (size_t i = 0; i < NUM_RANDOM_TESTS; ++i) {
            long long lhs = dis(rng);
            long long rhs = dis(rng);
            while (rhs == 0) rhs = dis(rng);
            CHECK(BigInt(lhs % rhs) == BigInt(lhs) % BigInt(rhs));
        }
    }
}

TEST_CASE("BigInt::operator<<", "[BigInt]") {
    SECTION("(+) << (+)") {
        CHECK(BigInt(0) << 0 == 0);
        CHECK(BigInt(0) << 1 == 0);
        CHECK(BigInt(1) << 0 == 1);
        CHECK(BigInt(1) << 1 == 2);

        CHECK(BigInt(2) << 10 == 2048);
        CHECK(BigInt(123) << 4 == 1968);
        CHECK(BigInt(5) << 55 == 180143985094819840LL);
        CHECK(BigInt(1234567890) << 9 == 632098759680LL);

        CHECK(BigInt(1234) << BigInt(321) ==
              BigInt("5271616004652806083350913570826454618839474761176219036229298688105238283226620877456670430559469568"));
    }

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis = std::uniform_int_distribution<long long>(0, 20);

        for (size_t i = 0; i < NUM_RANDOM_TESTS; ++i) {
            long long lhs = dis(rng);
            long long rhs = dis(rng);
            CHECK(BigInt(lhs << rhs) == BigInt(lhs) << BigInt(rhs));
        }
    }
}

TEST_CASE("BigInt::operator>>", "[BigInt]") {
    SECTION("(+) >> (+)") {
        CHECK(BigInt(0) >> 0 == 0);
        CHECK(BigInt(0) >> 1 == 0);
        CHECK(BigInt(1) >> 0 == 1);

        CHECK(BigInt(1) >> 0 == 1);
        CHECK(BigInt(2) >> 1 == 1);
        CHECK(BigInt(4) >> 2 == 1);
        CHECK(BigInt(8) >> 3 == 1);

        CHECK(BigInt(12345) >> 6 == 192);
        CHECK(BigInt(1234567890) >> 9 == 2411265);

        CHECK(BigInt("9832669330483809071367926297102810309779485703236475414705978560881507106897177670904211402264602446") >> 76 ==
              BigInt("130134295037135451221794315165446946109466472663220581233866604309085671619914"));
    }

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis_lhs = std::uniform_int_distribution<long long>(0, std::numeric_limits<long long>::max());
        auto dis_rhs = std::uniform_int_distribution<long long>(0, 32);

        for (size_t i = 0; i < NUM_RANDOM_TESTS; ++i) {
            long long lhs = dis_lhs(rng);
            long long rhs = dis_rhs(rng);
            CHECK(BigInt(lhs >> rhs) == BigInt(lhs) >> BigInt(rhs));
        }
    }
}

TEST_CASE("BigInt::abs", "[BigInt]") {
    CHECK(BigInt().abs() == BigInt("0"));

    CHECK(BigInt(1).abs() == BigInt("1"));
    CHECK(BigInt(1234567890).abs() == BigInt("1234567890"));

    CHECK(BigInt(-1).abs() == BigInt("1"));
    CHECK(BigInt(-1234567890).abs() == BigInt("1234567890"));
}

TEST_CASE("BigInt::pow", "[BigInt]") {
    SECTION("Edge Cases") {
        CHECK(BigInt(0).pow(1) == BigInt("0"));
        CHECK(BigInt(0).pow(2) == BigInt("0"));

        CHECK(BigInt(1).pow(0) == BigInt("1"));
        CHECK(BigInt(1).pow(1) == BigInt("1"));
        CHECK(BigInt(1).pow(2) == BigInt("1"));

        CHECK(BigInt(2).pow(1) == BigInt("2"));
        CHECK(BigInt(2).pow(2) == BigInt("4"));
        CHECK(BigInt(2).pow(3) == BigInt("8"));

        REQUIRE_THROWS(BigInt(1).pow(-1));

        CHECK(BigInt(461).pow(92) == BigInt("114943690214714631186310146240142733983651598647852463221116086763542857967992722059056778482563560919327095063942085548142184729470249742889637845401357798402737179498215788698770823739810376126874990642187561820379388460338430744095755386279921"));
    }

    SECTION("Random") {
        auto rng = std::mt19937(std::random_device{}());
        auto dis_base = std::uniform_int_distribution<long long>(-10, 10);
        auto dis_exp = std::uniform_int_distribution<long long>(0, 10);

        for (size_t i = 0; i < NUM_RANDOM_TESTS; ++i) {
            long long base = dis_base(rng);
            long long exp = dis_exp(rng);
            CHECK(BigInt(std::pow(base, exp)) == BigInt(base).pow(exp));
        }
    }
}