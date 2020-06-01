#include "rsa.h"
#include "catch.hpp"
#include <cassert>

TEST_CASE("string-number", "[rsa]")
{
	const char* strings[] = {
		"Test!",
		"Another test",
		"ASDsdg7w3",
		"\r\n\t123",
	};

	int constexpr STR_COUNT = sizeof(strings) / sizeof(strings[0]);

	for (int i = 0; i < STR_COUNT; i++) {
		String str = strings[i];
		BigSmoke num = rsa::stringToNumber(str.getCharAr());
		String str2 = rsa::numberToString(num);
		REQUIRE(str == str2);
	}
}

TEST_CASE("encrypt-decrypt", "[rsa]")
{
	rsa::Key key(BigSmoke("11111111111111111011"), BigSmoke("44444444443333332221"));

	const char* strings[] = {
		"Test!",
		"Another test",
		"ASDsdg7w3",
		"\r\n\t123",
	};

	int constexpr STR_COUNT = sizeof(strings) / sizeof(strings[0]);

	for (int i = 0; i < STR_COUNT; i++) {
		String str = strings[i];
		BigSmoke num = rsa::stringToNumber(str.getCharAr());
		REQUIRE(key.canBeEncrypted(num));
		BigSmoke encrypted = rsa::encrypt(num, key);
		BigSmoke decrypted = rsa::decryptAsNumber(encrypted, key);
		REQUIRE(num == decrypted);
	}
}
