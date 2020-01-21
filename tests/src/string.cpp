#include <cstring>
#include <algorithm>
#include "catch.hpp"

#include <BigNumber.h>

static char *reverse_str(char *buf)
{
    int len = strlen(buf);
    int len2 = len / 2;
    for (int i = 0; i < len2; i++)
    {
        std::swap(buf[i], buf[len - i - 1]);
    }
    return buf;
}

TEST_CASE("String constructors", "[string]") {
    SECTION("default constructor") {
        String a;
        REQUIRE(a.size() == 0);
    }

    SECTION("from char array") {
        const char *origCharArray = "A string of 20 chars";
        String str(origCharArray);
        REQUIRE(str.size() == 20);
        REQUIRE(!strcmp(origCharArray, str.getCharAr()));
    }

    SECTION("from another string") {
        const char *arr = "String with text";

        String str1(arr);
        REQUIRE(!strcmp(arr, str1.getCharAr()));

        String str2(str1);
        REQUIRE(!strcmp(arr, str2.getCharAr()));
    }

    SECTION("from single char") {
        for (char c = 'a'; c <= 'z'; c++)
        {
            String str(c);
            REQUIRE(str.getCharAr()[0] == c);
        }
    }
}

TEST_CASE("String begin(), end() and []", "[string]") {
    String str = "A string with text!";

    SECTION("iterators") {
        String str = "A string with text!";

        auto it = str.begin();
        auto it_end = str.end();
        int size = str.size();

        int i = 0;

        for (;;)
        {
            REQUIRE((it != it_end) == (i < str.size()));
            if (it == it_end)
                break;
            REQUIRE(*it == str[i]);

            it++;
            i++;
        }
    }

    SECTION("[] bounds check") {
        for (int i = -4; i < 0; i++)
        {
            REQUIRE_THROWS_AS(str[i], std::out_of_range);
        }

        for (int i = 0; i < str.size(); i++)
        {
            REQUIRE_NOTHROW(str[i]);
        }

        for (int i = str.size(); i < str.size() + 4; i++)
        {
            REQUIRE_THROWS_AS(str[i], std::out_of_range);
        }
    }
}

TEST_CASE("String.reverse()", "[string]") {
    SECTION("empty string") {
        String str1;
        String str2 = str1.reverse();
        REQUIRE(str2.size() == 0);
    }

    SECTION("non-empty string") {
        char buf[128];
        strcpy(buf, "A test string?");
        String str1 = buf;

        reverse_str(buf);
        String str2 = str1.reverse();

        REQUIRE(!strcmp(buf, str2.getCharAr()));
    }
}

#define OPERATOR_TEST_STR   "A string with text"
#define OPERATOR_TEST_STR2  "Test"

TEST_CASE("operators", "[string]") {
    String str1 = OPERATOR_TEST_STR;

    SECTION("comparison -- ==, !=") {
        String str2 = OPERATOR_TEST_STR;
        REQUIRE(str1 == str2);
        REQUIRE_FALSE(str1 != str2);

        String str3 = OPERATOR_TEST_STR2;
        REQUIRE(str1 != str3);
        REQUIRE_FALSE(str1 == str3);
    }

    SECTION("concatenation -- +, +=") {
        SECTION("empty string") {
            String str2;
            str2 = str1 + String();
            REQUIRE(str1 == str2);

            str2 = String() + str1;
            REQUIRE(str1 == str2);
        }

        SECTION("non-empty string") {
            String str2 = str1 + OPERATOR_TEST_STR2;
            REQUIRE(!strcmp(str2.getCharAr(), OPERATOR_TEST_STR OPERATOR_TEST_STR2));

            String str3 = str1;
            str3 += OPERATOR_TEST_STR2;
            REQUIRE(!strcmp(str3.getCharAr(), OPERATOR_TEST_STR OPERATOR_TEST_STR2));
        }
    }

    SECTION("repetition -- *, *=") {
        SECTION("0 times") {
            String str1 = OPERATOR_TEST_STR;

            String str2 = str1 * 0;
            REQUIRE(str2.size() == 0);

            str1 *= 0;
            REQUIRE(str1.size() == 0);
        }

        SECTION("1 time") {
            String str1 = OPERATOR_TEST_STR;

            String str2 = str1 * 1;
            REQUIRE(str2 == str1);

            str1 *= 1;
            REQUIRE(str1 == OPERATOR_TEST_STR);
        }

        SECTION("3 times") {
            String str1 = "Ab";

            String str2 = str1 * 3;
            REQUIRE(str2 == "AbAbAb");

            str1 *= 3;
            REQUIRE(str1 == "AbAbAb");
        }

        SECTION("Negative times") {
            String str1 = OPERATOR_TEST_STR;
            REQUIRE_THROWS_AS(str1 *= -1, std::invalid_argument);
        }
    }
}
