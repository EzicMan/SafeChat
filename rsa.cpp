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

rsa::Key::Key(const BigSmoke &p_val, const BigSmoke &q_val) : Key()
{
	p = p_val;
	q = q_val;
	n = p * q;

	// Calc lambda(n)
	l = CalcLCM(p - 1, q - 1);

	// Calc d
	ExtendedGCDOut out = CalcExtendedGCD(e, l, l);
	d = out.x;
}

BigSmoke rsa::StringToNumber(const char *data)
{
	// TODO: Remove to hex conversion
	String dataHex = "0x";

	for (uint8_t *c = (uint8_t *)data; *c; c++)
	{
		dataHex += s_HexTable[(*c & 0xF0) >> 4];
		dataHex += s_HexTable[(*c & 0x0F) >> 0];
	}

	return BigSmoke(dataHex);
}

BigSmoke rsa::Encrypt(const BigSmoke &m, const Key &key)
{
	if (m >= key.n)
		throw std::invalid_argument("message < key.n");

	return ModuloPower(m, key.e, key.n);
}

BigSmoke rsa::DecryptAsNumber(const BigSmoke &encr, const Key &key)
{
	return ModuloPower(encr, key.d, key.n);
}
