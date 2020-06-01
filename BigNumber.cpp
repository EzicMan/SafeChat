//                          _
//  _._ _..._ .-',     _.._(`))
// '-. `     '  /-._.-'    ',/
//    )         \            '.
//   / _    _    |             \
//  |  a    a    /              |
//  \   .-.                     ;
//   '-('' ).-'       ,'       ;
//      '-;           |      .'
//         \           \    /
//         | 7  .__  _.-\   \
//         | |  |  ``/  /`  /
//        /,_|  |   /,_/   /
//           /,_/      '`-'
//
// Safety Pig Igor warns you: this code is unreadable and full of kostils.
// Listen to Safety Pig Igor and close the source code file right now.
// If you really want to continue, please consider taking Igor with yourself in case
// you need immediate medical help.
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// And you WILL need it.
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// Memetic kill agent activated. 7 days.
//

#include <cassert>
#include <cmath>
#include <cstring>

#include "BigNumber.h"

long long Max(long long a, long long b)
{
	return a > b ? a : b;
}
long long Min(long long a, long long b)
{
	return a < b ? a : b;
}

BigSmoke Power(BigSmoke a, BigSmoke n)
{
	BigSmoke res = 1;
	while (n != 0)
		if (n % 2 != 0) {
			res *= a;
			--n;
		} else {
			a *= a;
			n /= 2;
		}
	return res;
}

//-----------------------------------------------------
// String
//-----------------------------------------------------
char String::m_sEmptyString[16] = {};

String::String(DummyStruct) {}

void String::_setCapacity(long long n)
{
	if (n > m_iCapacity) {
		if (m_iCapacity <= 0)
			m_iCapacity = 1;

		// This code doubles m_iCapacity enough times to be >= n.
		// m_iCapacity * 2^x >= n
		long long newCap = (n / m_iCapacity) + 1;
		newCap = 2LL << int(log2(newCap));
		newCap = m_iCapacity * newCap;

		assert(newCap >= n);

		_setExactCapacity(newCap);
	}
}

void String::_setExactCapacity(long long n)
{
	char* newMem = _allocateMemBlock(n + 1);
	if (string) {
		memcpy(newMem, string, sSize + 1);
		_freeMemBlock(string);
	}
	string = newMem;
	m_iCapacity = n;
}

void String::_freeStringMem()
{
	_freeMemBlock(string);
	string = nullptr;
	sSize = -1;
	m_iCapacity = -1;
}

char* String::_allocateMemBlock(long long n)
{
	return new char[n];
}

void String::_freeMemBlock(char* mem)
{
	if (mem != m_sEmptyString)
		delete[] mem;
}

String::String()
{
	string = m_sEmptyString;
	sSize = 0;
}

String::String(const char* a)
{
	long long i = 0;
	long long n = 0;
	while (true) {
		if (a[i] == '\0') {
			break;
		}
		n++;
		i++;
	}
	sSize = n;
	_setExactCapacity(sSize);
	memcpy(string, a, sSize + 1);
}

String::String(char a)
{
	_setExactCapacity(1);
	string[0] = a;
	string[1] = '\0';
	sSize = 1;
}

String::String(char a, long long times)
{
	_setExactCapacity(times);
	memset(string, a, times);
	string[times] = '\0';
	sSize = times;
}

String::String(const String& a)
{
	sSize = a.size();
	_setExactCapacity(sSize);
	memcpy(string, a.getCharAr(), sSize + 1);
}

String::String(String&& rhs) noexcept
{
	string = rhs.string;
	sSize = rhs.sSize;
	m_iCapacity = rhs.m_iCapacity;

	rhs.string = nullptr;
	rhs.sSize = -1;
	rhs.m_iCapacity = -1;
}

String::~String()
{
	_freeStringMem();
}

String& String::operator=(const String& rhs)
{
	if (&rhs == this)
		return *this;

	_freeStringMem();

	sSize = rhs.size();
	_setExactCapacity(sSize);
	memcpy(string, rhs.getCharAr(), sSize + 1);

	return *this;
}

String& String::operator=(String&& rhs) noexcept
{
	if (&rhs == this)
		return *this;

	_freeStringMem();

	string = rhs.string;
	sSize = rhs.sSize;
	m_iCapacity = rhs.m_iCapacity;

	rhs.string = nullptr;
	rhs.sSize = -1;
	rhs.m_iCapacity = -1;

	return *this;
}

String String::substring(long long sindex, long long eindex) const
{
	if (sindex < 0)
		sindex = 0;
	if (eindex >= sSize)
		eindex = sSize;
	if (eindex == sindex)
		return String("");
	if (sindex > eindex)
		return String("");

	String ret(DummyStruct {});
	ret.sSize = eindex - sindex;

	ret._setExactCapacity(ret.sSize);
	memcpy(ret.string, string + sindex, ret.sSize);
	ret.string[ret.sSize] = '\0';

	return ret;
}

String String::reverse() const
{
	String b(*this);

	long long len = size();
	long long len2 = len / 2;
	for (long long i = 0; i < len2; i++) {
		char t = b[i];
		b[i] = b[len - i - 1];
		b[len - i - 1] = t;
	}

	return b;
}

String String::toLower() const
{
	String b(*this);

	for (char& c : b) {
		if (c >= 'A' && c <= 'Z')
			c = c - 'A' + 'a';
	}

	return b;
}

String String::toUpper() const
{
	String b(*this);

	for (char& c : b) {
		if (c >= 'a' && c <= 'z')
			c = c - 'a' + 'A';
	}

	return b;
}

String& String::operator+=(const String& rhs)
{
	_setCapacity(sSize + rhs.sSize);
	memcpy(string + sSize, rhs.string, rhs.sSize + 1);
	sSize = sSize + rhs.sSize;

	return *this;
}

String& String::operator+=(char c)
{
	_setCapacity(sSize + 1);
	string[sSize] = c;
	string[sSize + 1] = '\0';
	sSize = sSize + 1;

	return *this;
}

bool String::operator==(const String& right) const
{
	if (right.size() != this->size()) {
		return false;
	}
	for (long long i = 0; i < right.size(); i++) {
		if (string[i] != right.getCharAr()[i]) {
			return false;
		}
	}
	return true;
}

bool String::operator!=(const String& right) const
{
	if (*this == right) {
		return false;
	}
	return true;
}

String& String::operator*=(long long times)
{
	if (times < 0) {
		throw std::invalid_argument("Can't multiply by negative numbers");
	}

	_setCapacity(sSize * times);

	char* ptr = string + sSize;
	for (int i = 0; i < times - 1; i++) {
		memcpy(ptr, string, sSize);
		ptr += sSize;
	}

	sSize = sSize * times;
	string[sSize] = '\0';

	return *this;
}

String operator*(const String& left, long long times)
{
	String c;
	c += left;
	c *= times;
	return c;
}

String operator*(long long times, const String& left)
{
	String c;
	c += left;
	c *= times;
	return c;
}

String operator+(const String& left, const String& right)
{
	String c;
	c += left;
	c += right;
	return c;
}

char& String::operator[](long long index) const
{
	if (index > sSize) {
		throw std::out_of_range("Invalid index");
	} else if (index < 0) {
		throw std::out_of_range("Invalid index");
	}
	return string[index];
}

std::ostream& operator<<(std::ostream& os, const String& r)
{
	os << r.getCharAr();
	return os;
}

std::istream& operator>>(std::istream& is, String& r)
{
	char a;
	String b;
	while (is.get(a)) {
		if (a == '\n') {
			break;
		}
		char c[2];
		c[0] = a;
		c[1] = '\0';
		b += c;
	}
	r = b;
	return is;
}

//-----------------------------------------------------
// BigSmoke
//-----------------------------------------------------
BigSmoke::BigSmoke()
{
	number = String("0");
}

BigSmoke::BigSmoke(const String a)
{
	if (a.size() == 0) {
		number = "0";
	} else {
		if (a.size() > 2 && a[0] == '0' && (a[1] == 'x' || a[1] == 'X')) {
			//from positive hex
			negative = false;
			String b = a.toLower();
			for (int i = 2; i < b.size(); i++) {
				if ((b[i] < '0' || b[i] > '9') && (b[i] < 'a' || b[i] > 'f')) {
					throw std::invalid_argument("string contains invalid chars");
				}
			}
			BigSmoke k = 0;
			BigSmoke tn = 0;
			for (long long i = b.size() - 1; i >= 2; i--) {
				if (b[i] >= 'a') {
					tn += BigSmoke(b[i] - 'a' + 10) * Power(16, k);
				} else {
					tn += (b[i] - '0') * Power(16, k);
				}
				k++;
			}
			number = tn.toString().reverse();
		} else if (a.size() > 3 && a[0] == '-' && a[1] == '0' && (a[2] == 'x' || a[2] == 'X')) {
			//from negtive hex
			negative = true;
			String b = a.toLower();
			for (int i = 3; i < b.size(); i++) {
				if ((b[i] < '0' || b[i] > '9') && (b[i] < 'a' || b[i] > 'f')) {
					throw std::invalid_argument("string contains invalid chars");
				}
			}
			BigSmoke k = 0;
			BigSmoke tn = 0;
			for (long long i = b.size() - 1; i >= 3; i--) {
				if (b[i] >= 'a') {
					tn += (b[i] - 'a' + 10) * Power(16, k);
				} else {
					tn += (b[i] - '0') * Power(16, k);
				}
				k++;
			}
			number = tn.toString().reverse();
			if (number == "0") {
				negative = false;
			}
		} else {
			if ((a[0] < '0' || a[0] > '9') && a[0] != '-') {
				throw std::invalid_argument("string contains invalid chars");
			}
			for (long long i = 0; i < a.size(); i++) {
				if (i != 0 && (a[i] < '0' || a[i] > '9')) {
					throw std::invalid_argument("string contains invalid chars");
				}
			}
			if (a[0] == '-') {
				negative = true;
				number = a.substring(1, a.size()).reverse();
				this->normalize();
				if (number == "0") {
					negative = false;
				}
			} else {
				number = a.reverse();
				this->normalize();
			}
		}
	}
}

BigSmoke::BigSmoke(const long long a)
{
	long long b = a;
	if (a < 0) {
		negative = true;
		b *= -1;
	}
	if (a == 0) {
		number = "0";
	}
	while (b > 0) {
		number += String(b % 10 + '0');
		b /= 10;
	}
}

String BigSmoke::toString() const
{
	String ans = number;
	if (negative) {
		ans += '-';
	}
	return ans.reverse();
}

String BigSmoke::asHexString(bool evenCharCount) const
{
	String ans = "";
	BigSmoke num = *this;
	while (num >= 16) {
		BigSmoke a = num % 16;
		if (a >= 10) {
			a -= 10;
			ans += ('A' + a.toString()[0] - '0');
		} else {
			ans += a.toString()[0];
		}
		num /= 16;
	}
	if (num >= 10) {
		num -= 10;
		ans += ('A' + num.toString()[0] - '0');
	} else {
		ans += num.toString()[0];
	}

	if (evenCharCount && ans.size() % 2 != 0)
		ans += '0';

	ans += "x0";
	return ans.reverse();
}

String BigSmoke::asBinaryString() const
{
	String ans = "";
	BigSmoke num = *this;
	while (num >= 2) {
		BigSmoke a = num % 2;
		ans += a.toString()[0];
		num /= 2;
	}
	ans += num.toString()[0];
	return ans.reverse();
}

BigSmoke BigSmoke::abs() const
{
	BigSmoke a;
	a.number = this->number;
	a.negative = false;
	return a;
}

const long long BigSmoke::size() const
{
	return number.size();
}

const bool BigSmoke::isNegative() const
{
	return negative;
}

void BigSmoke::normalize()
{
	long long i;
	for (i = number.size() - 1; i >= 0 && number[i] == '0'; i--)
		;
	if (i == -1) {
		number = "0";
	} else {
		number = number.substring(0, i + 1);
	}
}

BigSmoke& BigSmoke::operator+=(const BigSmoke& right)
{
	long long size1 = this->size();
	long long size2 = right.size();
	if (right.negative && !negative) {
		BigSmoke ans = (*this - BigSmoke(right.number.reverse()));
		number = ans.number;
		negative = ans.negative;
		return *this;
	} else if (!right.negative && negative) {
		BigSmoke ans = (right - BigSmoke(number.reverse()));
		number = ans.number;
		negative = ans.negative;
		return *this;
	}
	String newNum = String("0") * Max(size1, size2);
	long long ost = 0;
	for (long long i = 0; i < newNum.size(); i++) {
		long long cur1 = 0, cur2 = 0, sum = 0;
		if (i >= size1) {
			cur1 = 0;
		} else {
			cur1 = number[i] - '0';
		}
		if (i >= size2) {
			cur2 = 0;
		} else {
			cur2 = right.number[i] - '0';
		}
		sum = cur1 + cur2 + ost;
		newNum[i] = sum % 10 + '0';
		ost = sum / 10;
	}
	if (ost != 0) {
		newNum += "1";
	}
	number = newNum;
	return *this;
}

BigSmoke& BigSmoke::operator++()
{
	*this += 1;
	return *this;
}

BigSmoke& BigSmoke::operator--()
{
	*this -= 1;
	return *this;
}

BigSmoke BigSmoke::operator++(int)
{
	BigSmoke b = *this;
	++(*this);
	return b;
}

BigSmoke BigSmoke::operator--(int)
{
	BigSmoke b = *this;
	--(*this);
	return b;
}

BigSmoke& BigSmoke::operator*=(const BigSmoke& right)
{
	long long size1 = size();
	long long size2 = right.size();
	BigSmoke ans = 0;
	long long zeros = 0;
	int ost = 0;
	String curstep = String('0', size1 + size2);
	for (long long i = 0; i < size2; i++) {
		for (long long j = 0; j < size1; j++) {
			int num = (number[j] - '0') * (right.number[i] - '0') + ost + curstep[i + j] - '0';
			curstep[i + j] = num % 10 + '0';
			ost = num / 10;
		}
		curstep[size1 + i] = ost + '0';
		ost = 0;
	}
	ans = curstep.reverse();
	ans.negative = negative;
	*this = ans;
	if (right.negative) {
		this->negative = !this->negative;
	}
	if (*this == 0) {
		negative = false;
	}
	return *this;
}

BigSmoke& BigSmoke::operator-=(const BigSmoke& right)
{
	if (right.negative) {
		BigSmoke ans = (*this + BigSmoke(right.number.reverse()));
		number = ans.number;
		negative = ans.negative;
		return *this;
	}
	if (*this < right) {
		number = (right - *this).number;
		negative = true;
		return *this;
	}
	long long ost = 0;
	String ans = "";
	long long i;
	for (i = 0; i < right.size(); i++) {
		if ((number[i] - ost) < right.number[i]) {
			ans += (char)(number[i] + 10 - ost - right.number[i] + '0');
			ost = 1;
		} else {
			ans += (char)(number[i] - ost - right.number[i] + '0');
			ost = 0;
		}
	}
	while (i < this->size()) {
		if (number[i] == '0' && ost != 0) {
			ans += '9';
		} else if (ost != 0) {
			ans += number[i] - 1;
			ost = 0;
		} else {
			ans += number[i];
		}
		i++;
	}
	BigSmoke newNum = ans.reverse();
	newNum.normalize();
	number = newNum.number;
	return *this;
}

BigSmoke& BigSmoke::operator/=(const BigSmoke& right)
{
	long long size1 = this->size();
	long long size2 = right.size();
	if (right.negative) {
		this->negative = !this->negative;
	}
	if (right == 0) {
		throw std::runtime_error("Division by zero");
	}
	if (size1 < size2) {
		number = "0";
		negative = false;
		return *this;
	}
	BigSmoke divBy = right.abs();
	String ans = "";
	String ost = "";
	long long i = 0;
	int r = 0;
	String this_reversed = number.reverse();
	BigSmoke ost_big;
	BigSmoke temp;
	while (i < size1) {
		if (ost == "0") {
			ost = "";
		}
		r = ost.size();
		if (i + size2 - r > size1) {
			ans += String('0') * (size1 - i);
			break;
		}
		ost += this_reversed.substring(i, i + size2 - r);
		i += size2 - r;
		int k;
		k = size2 - r - 1;
		ost_big = ost;
		while (ost_big < divBy) {
			if (i >= size1) {
				break;
			}
			ost += this_reversed[i];
			i++;
			k++;
			ost_big = ost;
		}
		ans += String('0') * k;
		for (int i = 1; i <= 10; i++) {
			if (divBy * i > ost_big) {
				ans += ('0' + i - 1);
				ost_big -= divBy * (i - 1);
				break;
			}
		}
		ost = ost_big.number.reverse();
	}
	number = ans.reverse();
	if (negative && ost_big != 0) {
		*this -= 1;
	}

	normalize();

	return *this;
}

BigSmoke& BigSmoke::operator%=(const BigSmoke& right)
{
	if (right == 2) {
		*this = BigSmoke((this->number[0] - '0') % 2);
		return *this;
	}
	BigSmoke temp = *this / right;
	temp *= right;
	*this -= temp;
	return *this;
}

BigSmoke BigSmoke::operator-() const
{
	BigSmoke a;
	a.number = number;
	a.negative = !negative;
	return a;
}

BigSmoke BigSmoke::operator+() const
{
	return *this;
}

BigSmoke operator+(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c;
	c.number = left.number;
	c.negative = left.negative;
	c += right;
	return c;
}

BigSmoke operator*(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c;
	c.number = left.number;
	c.negative = left.negative;
	c *= right;
	return c;
}

BigSmoke operator/(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c;
	c.number = left.number;
	c.negative = left.negative;
	c /= right;
	return c;
}

BigSmoke operator%(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c;
	c.number = left.number;
	c.negative = left.negative;
	c %= right;
	return c;
}

BigSmoke operator-(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c;
	c.number = left.number;
	c.negative = left.negative;
	c -= right;
	return c;
}

bool BigSmoke::operator==(const BigSmoke& right) const
{
	if (number == "0" && right.number == "0") {
		return true;
	}
	if (number == right.number && negative == right.negative) {
		return true;
	}
	return false;
}

bool BigSmoke::operator!=(const BigSmoke& right) const
{
	if (*this == right) {
		return false;
	}
	return true;
}

bool BigSmoke::operator>(const BigSmoke& right) const
{
	if (right.negative && !negative) {
		return true;
	}
	if (!right.negative && negative) {
		return false;
	}
	if (right.negative && negative) {
		long long size1 = right.size();
		long long size2 = size();
		if (size1 > size2) {
			return true;
		} else if (size1 < size2) {
			return false;
		}
		for (long long i = size1 - 1; i >= 0; i--) {
			if (number[i] > right.number[i]) {
				return false;
			} else if (number[i] < right.number[i]) {
				return true;
			}
		}
		return false;
	}
	long long size1 = right.size();
	long long size2 = size();
	if (size1 > size2) {
		return false;
	} else if (size1 < size2) {
		return true;
	}
	for (long long i = size1 - 1; i >= 0; i--) {
		if (number[i] > right.number[i]) {
			return true;
		} else if (number[i] < right.number[i]) {
			return false;
		}
	}
	return false;
}

bool BigSmoke::operator<(const BigSmoke& right) const
{
	return right > *this;
}

bool BigSmoke::operator>=(const BigSmoke& right) const
{
	return (*this > right) || (*this == right);
}

bool BigSmoke::operator<=(const BigSmoke& right) const
{
	return (*this < right) || (*this == right);
}

std::ostream& operator<<(std::ostream& os, const BigSmoke& r)
{
	if (r.negative) {
		os << "-";
	}
	os << r.number.reverse().getCharAr();
	return os;
}

BigSmoke CalcGCD(BigSmoke a, BigSmoke b)
{
	while (b != 0) {
		a %= b;

		// swap(a, b);
		BigSmoke t = a;
		a = std::move(b);
		b = std::move(t);
	}
	return a;
}

BigSmoke CalcLCM(const BigSmoke& a, const BigSmoke& b)
{
	return a / CalcGCD(a, b) * b;
}

ExtendedGCDOut CalcExtendedGCD(const BigSmoke& a, const BigSmoke& b, const BigSmoke& m)
{
	if (a == 0)
		return ExtendedGCDOut { 0, 1, b };

	ExtendedGCDOut hren = CalcExtendedGCD(b % a, a, m);

	ExtendedGCDOut ret = ExtendedGCDOut {
		(hren.y - (ModuloDiv(b, a, m) * hren.x) % m) % m,
		hren.x % m,
		hren.gcd
	};
	return ret;
}

BigSmoke ModuloPower(BigSmoke a, BigSmoke n, const BigSmoke& m)
{
	a %= m;
	n %= m;

	BigSmoke res = 1;
	while (n != 0)
		if (n % 2 != 0) {
			res = (res * a) % m;
			--n;
		} else {
			a = (a * a) % m;
			n /= 2;
		}
	return res;
}

static BigSmoke inverse_element(const BigSmoke& x, const BigSmoke& m)
{
	return ModuloPower(x, m - 2, m);
}

BigSmoke ModuloDiv(const BigSmoke& a, const BigSmoke& b, const BigSmoke& m)
{
	return a * inverse_element(b, m) % m;
}
