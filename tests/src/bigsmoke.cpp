#include <cstring>
#include <cstdint>
#include <algorithm>
#include "catch.hpp"

#include <BigNumber.h>

TEST_CASE("BigSmoke constructors", "[bigsmoke]") {
    SECTION("default constructor") {
        BigSmoke a;
        REQUIRE(a == 0);
    }

    SECTION("string constructor") {
        BigSmoke a1("0");
        REQUIRE(a1 == 0);

        BigSmoke a2("1234");
        REQUIRE(a2 == 1234);

        BigSmoke a3("-1234");
        REQUIRE(a3 == -1234);
        REQUIRE(a3.isNegative());

        REQUIRE_THROWS_AS(BigSmoke("all we had to do was to follow the damn train cj"), std::invalid_argument);
        REQUIRE_THROWS_AS(BigSmoke("-12fg34"), std::invalid_argument);
        REQUIRE_THROWS_AS(BigSmoke("-12-34"), std::invalid_argument);

        BigSmoke a4("001234");
        REQUIRE(a4 == 1234);
    }

    SECTION("long long constructor") {
        BigSmoke a1(0);
        REQUIRE(a1 == 0);

        BigSmoke a2(1234);
        REQUIRE(a2 == 1234);

        BigSmoke a3(-1234);
        REQUIRE(a3 == -1234);

        BigSmoke a4(INT64_MIN);
        REQUIRE(a4 == INT64_MIN);

        BigSmoke a5(INT64_MAX);
        REQUIRE(a5 == INT64_MAX);
    }
}

TEST_CASE("string conversion", "[bigsmoke]") {
    BigSmoke a;
    REQUIRE(a.toString() == "0");

    a = BigSmoke("1234");
    REQUIRE(a.toString() == "1234");

    a = BigSmoke("-1234");
    REQUIRE(a.toString() == "-1234");
}

TEST_CASE("abs", "[bigsmoke]") {
    BigSmoke a(0);
    REQUIRE_FALSE(a.isNegative());

    a = 1234;
    REQUIRE_FALSE(a.isNegative());
    a = a.abs();
    REQUIRE_FALSE(a.isNegative());

    a = -1234;
    REQUIRE(a.isNegative());
    a = a.abs();
    REQUIRE_FALSE(a.isNegative());
}

TEST_CASE("addition -- +, +=", "[bigsmoke]") {
    BigSmoke a(0), b(0);

    b = a + 1000;
    a += 1000;
    REQUIRE(a == 1000);
    REQUIRE(b == 1000);
    REQUIRE(a == b);

    b = a + 999;
    a += 999;
    REQUIRE(a == 1999);
    REQUIRE(b == 1999);
    REQUIRE(a == b);

    b = a + 8001;
    a += 8001;
    REQUIRE(a == 10000);
    REQUIRE(b == 10000);
    REQUIRE(a == b);
}

TEST_CASE("substraction -- -, -=", "[bigsmoke]") {
    BigSmoke a(10000), b(10000);

    b = a - 1000;
    a -= 1000;
    REQUIRE(a == 9000);
    REQUIRE(b == 9000);
    REQUIRE(a == b);

    b = a - 999;
    a -= 999;
    REQUIRE(a == 8001);
    REQUIRE(b == 8001);
    REQUIRE(a == b);

    b = a - 10001;
    a -= 10001;
    REQUIRE(a == -2000);
    REQUIRE(b == -2000);
    REQUIRE(a == b);
    REQUIRE(a.isNegative());
    REQUIRE(b.isNegative());

    b = a + 2000;
    a += 2000;
    REQUIRE(a == 0);
    REQUIRE(b == 0);
    REQUIRE(a == b);
    REQUIRE_FALSE(a.isNegative());
    REQUIRE_FALSE(b.isNegative());
}

TEST_CASE("multiplication -- *, *=", "[bigsmoke]") {
    BigSmoke a(1), b(1);
    REQUIRE(a == 1);
    REQUIRE(b == 1);
    REQUIRE(a == b);

    b = a * 10;
    a *= 10;
    REQUIRE(a == 10);
    REQUIRE(b == 10);
    REQUIRE(a == b);

    b = a * 2;
    a *= 2;
    REQUIRE(a == 20);
    REQUIRE(b == 20);
    REQUIRE(a == b);

    b = a * (1234 / 2);
    a *= (1234 / 2);
    REQUIRE(a == 12340);
    REQUIRE(b == 12340);
    REQUIRE(a == b);

    b = a * -10;
    a *= -10;
    REQUIRE(a == -123400);
    REQUIRE(b == -123400);
    REQUIRE(a == b);
    REQUIRE(a.isNegative());
    REQUIRE(b.isNegative());

    b = a * -1;
    a *= -1;
    REQUIRE(a == 123400);
    REQUIRE(b == 123400);
    REQUIRE(a == b);
    REQUIRE_FALSE(a.isNegative());
    REQUIRE_FALSE(b.isNegative());

    b = a * -1;
    a *= -1;
    REQUIRE(a == -123400);
    REQUIRE(b == -123400);
    REQUIRE(a == b);
    REQUIRE(a.isNegative());
    REQUIRE(b.isNegative());

    b = a * 0;
    a *= 0;
    REQUIRE(a == 0);
    REQUIRE(b == 0);
    REQUIRE(a == b);
    REQUIRE_FALSE(a.isNegative());
    REQUIRE_FALSE(b.isNegative());
}

TEST_CASE("division -- /, /=", "[bigsmoke]") {
    BigSmoke a, b;

    SECTION("100 / 10 / (-10) = -1") {
        a = b = 100;

        b = a / 10;
        a /= 10;
        REQUIRE(a == 10);
        REQUIRE(b == 10);
        REQUIRE(a == b);
        REQUIRE_FALSE(a.isNegative());
        REQUIRE_FALSE(b.isNegative());

        b = a / -10;
        a /= -10;
        REQUIRE(a == -1);
        REQUIRE(b == -1);
        REQUIRE(a == b);
        REQUIRE(a.isNegative());
        REQUIRE(b.isNegative());
    }

    SECTION("3333 / 3 = 1111") {
        a = b = 3333;

        b = a / 3;
        a /= 3;

        REQUIRE(a == 1111);
        REQUIRE(b == 1111);
        REQUIRE(a == b);
    }

    SECTION("3 / 3 = 1") {
        a = b = 3;

        b = a / 3;
        a /= 3;

        REQUIRE(a == 1);
        REQUIRE(b == 1);
        REQUIRE(a == b);
    }

    SECTION("3 / 1 = 3") {
        a = b = 3;

        b = a / 1;
        a /= 1;

        REQUIRE(a == 3);
        REQUIRE(b == 3);
        REQUIRE(a == b);
    }
}

TEST_CASE("division remainder -- %, %/", "[bigsmoke]") {
    BigSmoke a, b;

    SECTION("100 % 10 = 0") {
        a = b = 100;

        b = a % 10;
        a %= 10;
        REQUIRE(a == 0);
        REQUIRE(b == 0);
        REQUIRE(a == b);
    }

    SECTION("333 % 3 = 0") {
        a = b = 333;

        b = a % 3;
        a %= 3;
        REQUIRE(a == 0);
        REQUIRE(b == 0);
        REQUIRE(a == b);
    }

    SECTION("25 % 24 = 1") {
        a = b = 25;

        b = a % 24;
        a %= 24;
        REQUIRE(a == 1);
        REQUIRE(b == 1);
        REQUIRE(a == b);
    }

    SECTION("0 % 0 = std::runtime_error") {
        a = b = 0;

        REQUIRE_THROWS_AS(a % 0, std::runtime_error);
        REQUIRE_THROWS_AS(a %= 0, std::runtime_error);
    }
}

TEST_CASE("unary operators -- +, -", "[bigsmoke]") {
    BigSmoke a = 10;
    REQUIRE(a == 10);
    REQUIRE_FALSE(a.isNegative());

    a = -a;
    REQUIRE(a == -10);
    REQUIRE(a.isNegative());

    a = -a;
    REQUIRE(a == 10);
    REQUIRE_FALSE(a.isNegative());

    a = +a;
    REQUIRE(a == 10);
    REQUIRE_FALSE(a.isNegative());
}
