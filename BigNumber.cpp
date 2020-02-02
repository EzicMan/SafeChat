#include "BigNumber.h"
#include <cstring>

long long Max(long long a, long long b)
{
	return a > b ? a : b;
}

BigSmoke Power(BigSmoke a, BigSmoke b)
{
	BigSmoke c = 1;
	for (BigSmoke i = 0; i < b; i++) {
		c *= a;
	}
	return c;
}

//-----------------------------------------------------
// String
//-----------------------------------------------------
String::String()
{
	string = new char[1];
	string[0] = '\0';
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
	string = new char[sSize + 1];
	memcpy(string, a, sSize + 1);
}

String::String(char a)
{
	string = new char[2];
	string[0] = a;
	string[1] = '\0';
	sSize = 1;
}

String::String(const String& a)
{
	sSize = a.size();
	const char* b = a.getCharAr();
	string = new char[sSize + 1];
	memcpy(string, b, sSize + 1);
}

String::String(String&& rhs) noexcept
{
	string = rhs.string;
	sSize = rhs.sSize;

	rhs.string = new char[1];
	rhs.string[0] = '\0';
	rhs.sSize = 0;
}

String::~String()
{
	delete[] string;
}

String& String::operator=(const String& rhs)
{
	if (&rhs == this)
		return *this;

	delete[] string;

	sSize = rhs.size();
	string = new char[sSize + 1];
	memcpy(string, rhs.getCharAr(), sSize + 1);

	return *this;
}

String& String::operator=(String&& rhs) noexcept
{
	if (&rhs == this)
		return *this;

	delete[] string;

	string = rhs.string;
	sSize = rhs.sSize;

	rhs.string = new char[1];
	rhs.string[0] = '\0';
	rhs.sSize = 0;

	return *this;
}

String String::substring(long long sindex, long long eindex) const
{
	String ans = "";
	for (long long i = sindex; i < eindex; i++) {
		ans += string[i];
	}
	return ans;
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

String& String::operator+=(const String& right)
{
	const char* a = right.getCharAr();
	const long long aSize = right.size();
	char* oldString = string;
	string = new char[sSize + aSize + 1];
	memcpy(string, oldString, sSize);
	delete[] oldString;
	for (long long i = sSize; i <= sSize + aSize; i++) {
		string[i] = a[i - sSize];
	}
	sSize = sSize + aSize;
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
	char* oldString = string;
	string = new char[sSize * times + 1];
	if (times == 0) {
		string[0] = '\0';
	}
	for (long long i = 0; i < times; i++) {
		memcpy(string + i * sSize, oldString, sSize + 1);
	}
	delete[] oldString;
	sSize = sSize * times;
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

BigSmoke::BigSmoke(const BigSmoke& a)
{
	number = a.number;
	negative = a.negative;
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

String BigSmoke::asHexString() const
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
	ans += "x0";
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
	String num = "";
	bool s = false;
	for (long long i = this->size() - 1; i >= 0; i--) {
		if (number[i] != '0' && !s) {
			s = true;
		}
		if (s) {
			num += number[i];
		}
	}
	if (num == "") {
		number = "0";
	} else {
		number = num.reverse();
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
	long long size1 = this->size();
	long long size2 = right.size();
	if (right.negative) {
		this->negative = !this->negative;
	}
	BigSmoke newNum = 0;
	long long zeros = 0;
	for (long long i = 0; i < size2; i++) {
		long long ost = 0;
		String curstate;
		if (zeros != 0) {
			curstate = String("0") * zeros;
		} else {
			curstate = "";
		}
		for (long long j = 0; j < size1; j++) {
			long long num = (number[j] - '0') * (right.number[i] - '0');
			num += ost;
			ost = num / 10;
			num %= 10;
			curstate += (char)(num + '0');
		}
		if (ost != 0) {
			curstate += (char)(ost + '0');
		}
		newNum += BigSmoke(curstate.reverse());
		zeros++;
	}
	newNum.normalize();
	number = newNum.number;
	if (number == "0") {
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
	long long k = 0;
	String ost = "";
	long long i = 0;
	long long z = 0;
	while (i < number.size()) {
		while (BigSmoke(ost) < divBy && i < number.size()) {
			ost += number.reverse()[i];
			i++;
			z++;
		}
		if (BigSmoke(ost) < divBy) {
			ans += String("0") * z;
			break;
		}
		if (ans != "") {
			ans += String("0") * (z - 1);
		}
		z = 0;
		BigSmoke temp = 0;
		for (long long j = 0; j <= 10; j++) {
			temp = divBy * j;
			if (temp > BigSmoke(ost)) {
				ans += (char)(j - 1 + '0');
				temp = divBy * (j - 1);
				break;
			}
		}
		temp = BigSmoke(ost) - temp;
		ost = temp.number.reverse();
	}
	number = ans.reverse();
	if (negative && BigSmoke(ost) != 0) {
		*this -= 1;
	}
	return *this;
}

BigSmoke& BigSmoke::operator%=(const BigSmoke& right)
{
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
	BigSmoke c("0");
	c += left;
	c += right;
	return c;
}

BigSmoke operator*(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c = 1;
	c *= left;
	c *= right;
	return c;
}

BigSmoke operator/(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c = 1;
	c *= left;
	c /= right;
	return c;
}

BigSmoke operator%(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c = 1;
	c *= left;
	c %= right;
	return c;
}

BigSmoke operator-(const BigSmoke& left, const BigSmoke& right)
{
	BigSmoke c = 0;
	c += left;
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
