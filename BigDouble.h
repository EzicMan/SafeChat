#pragma once
#include "BigNumber.h"

class BigDouble {
private:
	int precision;
	BigSmoke wholePart;
	BigSmoke doublePart;

public:
	BigDouble();
	BigDouble(long double a, int precision);
	BigDouble(BigSmoke num);
	BigDouble(String num);
	BigDouble(const char* num);

	void setPrecision(int precision);
	int getPrecision()
	{
		return precision;
	}
	BigDouble powerBy(BigSmoke n);
	String toString();

	friend BigDouble operator+(const BigDouble& left, const BigDouble& right);
	BigDouble& operator+=(const BigDouble& right);
	friend BigDouble operator-(const BigDouble& left, const BigDouble& right);
	BigDouble& operator-=(const BigDouble& right);
	friend BigDouble operator/(const BigDouble& left, const BigDouble& right);
	BigDouble& operator/=(const BigDouble& right);
	friend BigDouble operator*(const BigDouble& left, const BigDouble& right);
	BigDouble& operator*=(const BigDouble& right);

	bool operator==(const BigDouble& right) const;
	bool operator!=(const BigDouble& right) const;
	bool operator>(const BigDouble& right) const;
	bool operator<(const BigDouble& right) const;
	bool operator>=(const BigDouble& right) const;
	bool operator<=(const BigDouble& right) const;
	BigDouble operator-() const;
	BigDouble operator+() const;

	friend std::ostream& operator<<(std::ostream& os, const BigDouble& r);
};