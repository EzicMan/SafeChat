#include <cassert>
#include <exception>

#include "rsa.h"

static const BigSmoke s_StandartE = String("0x010001"); // 2^16 + 1

static const char s_HexTable[16] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'A', 'B',
	'C', 'D', 'E', 'F'
};

//-----------------------------------------------------
// RSA Key
//-----------------------------------------------------
rsa::Key::Key()
{
	e = s_StandartE;
}

rsa::Key::Key(const BigSmoke& p_val, const BigSmoke& q_val)
    : Key()
{
	p = p_val;
	q = q_val;
	n = p * q;

	// Calc d
	BigSmoke t = (p - 1) * (q - 1);
	d = calcD(e, t);

	// Calc max msg size
	{
		String smsg = "0xFF";
		BigSmoke msg = smsg;
		for (maxMsgSize = 1; msg < n; maxMsgSize++, smsg += "FF", msg = smsg)
			;
		maxMsgSize--;
	}
}

bool rsa::Key::canBeEncrypted(const BigSmoke& msg) const
{
	return msg < n;
}

BigSmoke rsa::Key::calcD(BigSmoke e, BigSmoke t)
{
	BigSmoke d;
	BigSmoke k = 1;

	while (true) {
		k = k + t;

		if (k % e == 0) {
			d = (k / e);
			return d;
		}
	}
}

//-----------------------------------------------------
// RSA
//-----------------------------------------------------
BigSmoke rsa::stringToNumber(const char* data)
{
	// TODO: Remove to hex conversion
	String dataHex = "0x";

	for (uint8_t* c = (uint8_t*)data; *c; c++) {
		dataHex += s_HexTable[(*c & 0xF0) >> 4];
		dataHex += s_HexTable[(*c & 0x0F) >> 0];
	}

	dataHex += "00";

	return BigSmoke(dataHex);
}

String rsa::numberToString(const BigSmoke& num)
{
	String out;
	String hex = num.asHexString(true);

	for (int i = 2; i < hex.size(); i += 2) {
		char buf[] = { hex[i], hex[i + 1], '\0' };
		char c = (char)(strtol(buf, nullptr, 16));
		if (c == '\0')
			break;
		out += c;
	}

	return out;
}

BigSmoke rsa::encrypt(const BigSmoke& m, const Key& key)
{
	if (!key.canBeEncrypted(m))
		throw std::invalid_argument("message is too big for modulus");

	return powerByModulus(m, key.e, key.n);
}

BigSmoke rsa::decryptAsNumber(const BigSmoke& encr, const Key& key)
{
	return powerByModulus(encr, key.d, key.n);
}
