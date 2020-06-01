#pragma once
#include "BigNumber.h"
#include "Vector.h"

namespace rsa {
class Key {
public:
	/**
	 * p: prime 1 - private
	 * q: prime 2 - private
	 * n: modulus - public
	 * e: public exponent
	 * d: private exponent
	 *
	 * c - encrypted message
	 * m - decrypted message
	**/

	/**
	 * Private keys
	 */
	BigSmoke p, q, d;

	/**
	 * Public keys
	 */
	BigSmoke n, e;

	/**
	 * Maximum size of a message of N 0xFF bytes
	 */
	long long maxMsgSize = 0;

	Key();
	Key(const BigSmoke& p_val, const BigSmoke& q_val);

	/**
	 * Returns true if specified message can be encrypted with this key (that is, msg < n).
	 */
	bool canBeEncrypted(const BigSmoke& msg) const;

private:
	static BigSmoke calcD(BigSmoke e, BigSmoke t);
};

/**
 * Takes a null-terminated sequence of bytes and converts them into BigSmoke.
 * Keeps a null byte at the end of the output.
 */
BigSmoke stringToNumber(const char* data);

/**
 * Takes a number created by StringToNumber and converts it back to a string.
 */
String numberToString(const BigSmoke& num);

/**
 * Encrypts big number with a key and returns encrypted message as a large number.
 * Key needs to have e and n set.
 */
BigSmoke encrypt(const BigSmoke& m, const Key& key);

/**
 * Decrypts data with a key and returns decrypted message as a large number.
 * Key needs to have d and n set.
 */
BigSmoke decryptAsNumber(const BigSmoke& encr, const Key& key);
}
