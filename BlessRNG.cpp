#include "BlessRNG.h"

BlessRNG::BlessRNG()
{
	seed = time(NULL);
	M = genSimple(128);
}

BlessRNG::BlessRNG(BigSmoke seed)
{
	this->seed = seed;
	M = genSimple(128);
}

BigSmoke BlessRNG::getNum(String bin)
{
	BigSmoke a = 0;
	BigSmoke k = 1;
	for (int i = 0; i < bin.size(); i++) {
		a += (bin[i] - '0') * k;
		k *= 2;
	}
	return a;
}

BigSmoke BlessRNG::getRandom(int sizeBits)
{
	String a = "";
	for (int i = 0; i < sizeBits; i++) {
		seed = PowerByModulus(seed, 2, M);
		a += (seed % 2).toString();
	}
	return getNum(a);
}

BigSmoke BlessRNG::genSimple(int bits)
{
	BigSmoke a = getRandom(bits);
	if (a % 2 == 0) {
		a++;
	}
	bool simple = false;
	while (!simple) {
		simple = true;
		for (int i = 2; i <= 1000; i++) {
			if (a % i == 0) {
				simple = false;
				a += 2;
				break;
			}
		}
		if (simple) {
			for (int i = 2; i <= 13; i++) {
				if (PowerByModulus(i, a - 1, a) != 1) {
					a += 2;
					simple = false;
					break;
				}
			}
		}
	}
	return a;
}