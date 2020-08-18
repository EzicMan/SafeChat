#include "BigDouble.h"
#include "BigNumber.h"

BigDouble::BigDouble()
{
	precision = 0;
	wholePart = 0;
	doublePart = 0;
}

BigDouble::BigDouble(long double a, int precision)
{
	this->precision = precision;
	wholePart = a;
	long double b = (long double)(a - (int)a);
	for (int i = 0; i < precision; i++) {
		b *= 10;
	}
	doublePart = b;
	bool neg = doublePart.isNegative();
	doublePart = doublePart.abs().toString().reverse();
	if (neg) {
		doublePart = -doublePart;
	}
}

BigDouble::BigDouble(BigSmoke num)
{
	this->precision = 0;
	wholePart = num;
	doublePart = 0;
}

BigDouble::BigDouble(String num)
{
	num = num.replaceAll(',', '.', 0);
	if (num.countSymbol('.', 0) > 1) {
		throw std::invalid_argument("Invalid input string: not a number");
	}
	long long i = num.findFirst('.', 0);
	if (i < 0) {
		wholePart = num;
		doublePart = 0;
		precision = 0;
	} else {
		wholePart = num.substring(0, i);
		doublePart = num.substring(i + 1, num.size()).reverse();
		precision = num.size() - i - 1;
	}
	if (wholePart.isNegative()) {
		doublePart = -doublePart;
	}
}

BigDouble::BigDouble(const char* num)
    : BigDouble(String(num))
{
}

BigDouble& BigDouble::operator+=(const BigDouble& right)
{
	int prec = Max(precision, right.precision);
	wholePart *= Power(10, prec);
	String doubt1;
	if (doublePart.isNegative()) {
		doubt1 += "-";
	}
	doubt1 += doublePart.abs().toString().reverse() + String('0') * (prec - doublePart.size());
	wholePart += doubt1;
	String doubt2;
	if (right.doublePart.isNegative()) {
		doubt2 += "-";
	}
	doubt2 += right.doublePart.abs().toString().reverse() + String('0') * (prec - right.doublePart.size());
	BigSmoke a = right.wholePart * Power(10, prec) + doubt2;
	precision = prec;
	wholePart += a;
	doublePart = 0;
	*this /= Power(10, precision);
	return *this;
}

BigDouble& BigDouble::operator-=(const BigDouble& right)
{
	int prec = Max(precision, right.precision);
	wholePart *= Power(10, prec);
	String doubt1;
	if (doublePart.isNegative()) {
		doubt1 += "-";
	}
	doubt1 += doublePart.abs().toString().reverse() + String('0') * (prec - doublePart.size());
	wholePart += doubt1;
	String doubt2;
	if (right.doublePart.isNegative()) {
		doubt2 += "-";
	}
	doubt2 += right.doublePart.abs().toString().reverse() + String('0') * (prec - right.doublePart.size());
	BigSmoke a = right.wholePart * Power(10, prec) + doubt2;
	precision = prec;
	wholePart -= a;
	doublePart = 0;
	*this /= Power(10, precision);
	return *this;
}

BigDouble& BigDouble::operator/=(const BigDouble& right)
{
	int prec = Max(precision, right.precision);
	bool neg = false;
	if (wholePart.isNegative() != doublePart.isNegative()) {
		neg = true;
	}
	wholePart = wholePart.abs();
	wholePart *= Power(10, prec);
	wholePart += doublePart.abs().toString().reverse() + String('0') * (prec - doublePart.size());
	BigSmoke a = right.wholePart.abs() * Power(10, prec) + (right.doublePart.abs().toString().reverse() + String('0') * (prec - right.doublePart.size()));
	wholePart *= Power(10, precision);
	wholePart /= a;
	doublePart = wholePart.abs().toString().substring(wholePart.size() - precision, wholePart.size()).reverse();
	wholePart = wholePart.abs().toString().substring(0, wholePart.size() - precision);
	if (neg) {
		wholePart = -wholePart;
		doublePart = -doublePart;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, const BigDouble& r)
{
	os << r.wholePart.toString().getCharAr() << '.' << r.doublePart.abs().toString().reverse().getCharAr() << String('0') * (r.precision - r.doublePart.size());
	return os;
}

void BigDouble::setPrecision(int precision)
{
	this->precision = precision;
	if (doublePart.size() > precision) {
		doublePart = doublePart.toString().substring(doublePart.size() - precision, doublePart.size());
	}
}

BigDouble operator+(const BigDouble& left, const BigDouble& right)
{
	BigDouble c;
	c.wholePart = left.wholePart;
	c.doublePart = left.doublePart;
	c.precision = left.precision;
	c += right;
	return c;
}

BigDouble operator-(const BigDouble& left, const BigDouble& right)
{
	BigDouble c;
	c.wholePart = left.wholePart;
	c.doublePart = left.doublePart;
	c.precision = left.precision;
	c -= right;
	return c;
}

BigDouble operator/(const BigDouble& left, const BigDouble& right)
{
	BigDouble c;
	c.wholePart = left.wholePart;
	c.doublePart = left.doublePart;
	c.precision = left.precision;
	c /= right;
	return c;
}

BigDouble& BigDouble::operator*=(const BigDouble& right)
{
	int prec = Max(precision, right.precision);
	String doubt1;
	if (doublePart.isNegative()) {
		doubt1 += "-";
	}
	doubt1 += doublePart.abs().toString().reverse() + String('0') * (prec - doublePart.size());
	BigSmoke a = wholePart * Power(10, prec) + doubt1;
	String doubt2;
	if (right.doublePart.isNegative()) {
		doubt2 += "-";
	}
	doubt2 += right.doublePart.abs().toString().reverse() + String('0') * (prec - right.doublePart.size());
	BigSmoke b = right.wholePart * Power(10, prec) + doubt2;
	precision = prec;
	a *= b;
	wholePart = a;
	doublePart = 0;
	*this /= Power(10, precision + precision);
	return *this;
}

BigDouble operator*(const BigDouble& left, const BigDouble& right)
{
	BigDouble c;
	c.wholePart = left.wholePart;
	c.doublePart = left.doublePart;
	c.precision = left.precision;
	c *= right;
	return c;
}

BigDouble BigDouble::powerBy(BigSmoke n)
{
	BigDouble res = BigSmoke(1);
	BigDouble a = *this;
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

bool BigDouble::operator==(const BigDouble& right) const
{
	if (wholePart == right.wholePart && doublePart == right.doublePart) {
		return true;
	}
	return false;
}

bool BigDouble::operator!=(const BigDouble& right) const
{
	if (*this == right) {
		return false;
	}
	return true;
}

bool BigDouble::operator>(const BigDouble& right) const
{
	if (wholePart > right.wholePart) {
		return true;
	}
	if (wholePart == right.wholePart && doublePart > right.doublePart) {
		return true;
	}
	return false;
}

bool BigDouble::operator<(const BigDouble& right) const
{
	if (!(*this > right)) {
		return true;
	}
	return false;
}

bool BigDouble::operator>=(const BigDouble& right) const
{
	if (*this > right || *this == right) {
		return true;
	}
	return false;
}

bool BigDouble::operator<=(const BigDouble& right) const
{
	if (*this < right || *this == right) {
		return true;
	}
	return false;
}

BigDouble BigDouble::operator-() const
{
	BigDouble a;
	a.wholePart = wholePart;
	a.doublePart = doublePart;
	a.precision = precision;
	return a;
}

BigDouble BigDouble::operator+() const
{
	return *this;
}

String BigDouble::toString()
{
	String ans;
	if (wholePart.isNegative()) {
		ans += '-';
	}
	ans += wholePart.abs().toString() + String('.') + doublePart.abs().toString().reverse();
	return ans;
}