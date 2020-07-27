#pragma once
#include "BigNumber.h"
#include <ctime>

class BlessRNG {
	BigSmoke seed = 0;
	BigSmoke M = 12709189;
	BigSmoke getNum(String bin);

public:
	BlessRNG();
	BlessRNG(BigSmoke seed);
	BigSmoke getRandom(int sizeBits);
	BigSmoke genSimple(int sizeBits);
};