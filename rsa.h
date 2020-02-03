#pragma once
#include "BigNumber.h"
#include "Vector.h"

namespace rsa {
class Key {
public:
	// p: prime 1 - private
	// q: prime 2 - private
	// n: modulus - public
	// e: public exponent
	// d: private exponent
	// l: lambda(n) - Carmichael's totient function (whatever it is) - private
	//
	// c - encrypted message
	// m - decrypted message

	// Private keys
	BigSmoke p, q, d, l;

	// Public keys
	BigSmoke n, e;

	Key();
	Key(const BigSmoke& p_val, const BigSmoke& q_val);
};

/**
 * Takes a null-terminated sequence of bytes and converts them into BigSmoke.
 */
BigSmoke StringToNumber(const char* data);

/**
 * Encrypts big number with a key and returns encrypted message as a large number.
 * Key needs to have e and n set.
 */
BigSmoke Encrypt(const BigSmoke& m, const Key& key);

/**
 * Encrypts data with a key and returns decrypted message as a large number.
 * Key needs to have d and n set.
 */
BigSmoke DecryptAsNumber(const BigSmoke& encr, const Key& key);
}