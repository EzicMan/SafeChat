#include <string>
#include "catch.hpp"

#include <BigNumber.h>

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
}