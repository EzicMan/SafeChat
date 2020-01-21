#include "BigNumber.h"

long long Max(long long a, long long b) {
	return a > b ? a : b;
}

//-----------------------------------------------------
// String
//-----------------------------------------------------
String::String() {
	string = new char[1];
	string[0] = '\0';
	sSize = 0;
}

String::String(char* a) {
	int i = 0;
	int n = 0;
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

String::String(const char* a) {
	int i = 0;
	int n = 0;
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

String::String(char a) {
	string = new char[2];
	string[0] = a;
	string[1] = '\0';
	sSize = 1;
}

String::String(const String& a) {
	sSize = a.size();
	const char* b = a.getCharAr();
	string = new char[sSize + 1];
	memcpy(string, b, sSize + 1);
}

String::~String() {
	delete[] string;
}

const int String::size() const {
	return sSize;
}

const char*String::begin() const {
	return string;
}

const char*String::end() const {
	return (string + sSize);
}

const char*String::getCharAr() const {
	return string;
}

String String::substring(int sindex, int eindex) const {
	String ans = "";
	for (int i = sindex; i < eindex; i++) {
		ans += string[i];
	}
	return ans;
}

String String::reverse() const
{
	String b;
	for (int i = size() - 1; i >= 0; i--) {
		b += String((*this)[i]);
	}
	return b;
}

String&String::operator+=(const String& right) {
	const char* a = right.getCharAr();
	const int aSize = right.size();
	char* oldString = string;
	string = new char[sSize + aSize + 1];
	memcpy(string, oldString, sSize);
	delete[] oldString;
	for (int i = sSize; i <= sSize + aSize; i++) {
		string[i] = a[i - sSize];
	}
	sSize = sSize + aSize;
	return *this;
}

String& String::operator=(const String& right) {
	const char* a = right.getCharAr();
	const int aSize = right.size();
	char* oldString = string;
	string = new char[aSize + 1];
	memcpy(string, a, aSize + 1);
	delete[] oldString;
	sSize = aSize;
	return *this;
}

bool String::operator==(const String& right) const {
	if (right.size() != this->size()) {
		return false;
	}
	for (int i = 0; i < right.size(); i++) {
		if (string[i] != right.getCharAr()[i]) {
			return false;
		}
	}
	return true;
}

bool String::operator!=(const String& right) const {
	if (*this == right) {
		return false;
	}
	return true;
}

String& String::operator*=(int times) {
	if (times < 0) {
		throw std::invalid_argument("Can't multiply by negative numbers");
	}
	char* oldString = string;
	string = new char[sSize * times + 1];
	if (times == 0) {
		string[0] = '\0';
	}
	for (int i = 0; i < times; i++) {
		memcpy(string + i * sSize, oldString, sSize + 1);
	}
	delete[] oldString;
	sSize = sSize * times;
	return *this;
}

String operator*(const String& left, int times) {
	String c;
	c += left;
	c *= times;
	return c;
}

String operator*(int times, const String& left) {
	String c;
	c += left;
	c *= times;
	return c;
}

String operator+(const String& left, const String& right) {
	String c;
	c += left;
	c += right;
	return c;
}

char& String::operator[](int index) const {
	if (index >= sSize) {
		throw std::out_of_range("Invalid index");
	}
	else if (index < 0) {
		throw std::out_of_range("Invalid index");
	}
	return string[index];
}

std::ostream& operator<<(std::ostream& os, const String& r)
{
	os << r.getCharAr();
	return os;
}

std::istream& operator>>(std::istream& is, String& r) {
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
BigSmoke::BigSmoke() {
	number = String("0");
}

BigSmoke::BigSmoke(const BigSmoke& a) {
	number = a.number;
	negative = a.negative;
}

BigSmoke::BigSmoke(const String a) {
	if (a.size() == 0) {
		number = "0";
	}
	else {
		if ((a[0] < '0' || a[0] > '9') && a[0] != '-') {
			throw std::invalid_argument("string contains invalid chars");
		}
		for (int i = 0; i < a.size(); i++) {
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
		}
		else {
			number = a.reverse();
			this->normalize();
		}
	}
}

BigSmoke::BigSmoke(const long long a) {
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

String BigSmoke::toString() const {
	return number.reverse();
}

BigSmoke BigSmoke::abs() const {
	BigSmoke a;
	a.number = this->number;
	a.negative = false;
	return a;
}

const int BigSmoke::size() const {
	return number.size();
}

const bool BigSmoke::isNegative() const {
	return negative;
}

void BigSmoke::normalize() {
	String num = "";
	bool s = false;
	for (int i = this->size() - 1; i >= 0; i--) {
		if (number[i] != '0' && !s) {
			s = true;
		}
		if (s) {
			num += number[i];
		}
	}
	if (num == "") {
		number = "0";
	}
	else {
		number = num.reverse();
	}
}

BigSmoke& BigSmoke::operator+=(const BigSmoke& right) {
	int size1 = this->size();
	int size2 = right.size();
	if (right.negative && !negative) {
		BigSmoke ans = (*this - BigSmoke(right.number.reverse()));
		number = ans.number;
		negative = ans.negative;
		return *this;
	}
	else if (!right.negative && negative) {
		BigSmoke ans = (right - BigSmoke(number.reverse()));
		number = ans.number;
		negative = ans.negative;
		return *this;
	}
	String newNum = String("0") * Max(size1, size2);
	int ost = 0;
	for (int i = 0; i < newNum.size(); i++) {
		int cur1 = 0, cur2 = 0, sum = 0;
		if (i >= size1) {
			cur1 = 0;
		}
		else {
			cur1 = number[i] - '0';
		}
		if (i >= size2) {
			cur2 = 0;
		}
		else {
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

BigSmoke& BigSmoke::operator*=(const BigSmoke& right) {
	int size1 = this->size();
	int size2 = right.size();
	if (right.negative) {
		this->negative = !this->negative;
	}
	BigSmoke newNum = 0;
	int zeros = 0;
	for (int i = 0; i < size2; i++) {
		int ost = 0;
		String curstate;
		if (zeros != 0) {
			curstate = String("0") * zeros;
		}
		else {
			curstate = "";
		}
		for (int j = 0; j < size1; j++) {
			int num = (number[j] - '0') * (right.number[i] - '0');
			num += ost;
			ost = num / 10;
			num %= 10;
			curstate += (num + '0');
		}
		if (ost != 0) {
			curstate += (ost + '0');
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

BigSmoke& BigSmoke::operator-=(const BigSmoke& right) {
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
	int ost = 0;
	String ans = "";
	int i;
	for (i = 0; i < right.size(); i++) {
		if (number[i] < right.number[i]) {
			ans += number[i] + 10 - ost - right.number[i] + '0';
			ost = 1;
		}
		else {
			ans += number[i] - right.number[i] + '0';
		}
	}
	while (i < this->size()) {
		if (number[i] == '0' && ost != 0) {
			ans += '9';
		}
		else if (ost != 0) {
			ans += number[i] - 1;
			ost = 0;
		}
		else {
			ans += number[i];
		}
		i++;
	}
	BigSmoke newNum = ans.reverse();
	newNum.normalize();
	number = newNum.number;
	return *this;
}

BigSmoke& BigSmoke::operator/=(const BigSmoke& right) {
	int size1 = this->size();
	int size2 = right.size();
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
	BigSmoke newNum = number.reverse();
	String ans = "";
	while (newNum >= divBy) {
		int i = 0;
		String a = "";
		while (BigSmoke(a) < divBy) {
			a += newNum.number.reverse()[i];
			i++;
		}
		BigSmoke temp = 0;
		for (int j = 0; j <= 10; j++) {
			temp = divBy * j;
			if (temp > BigSmoke(a)) {
				ans += j - 1 + '0';
				temp = divBy * (j - 1);
				break;
			}
		}
		temp = BigSmoke(a) - temp;
		a = temp.number.reverse();
		for (; i < newNum.size(); i++) {
			a += newNum.number.reverse()[i];
		}
		temp = a;
		if (temp == 0 && a.size() != 1) {
			ans += String("0") * (a.size() - 1);
		}
		newNum.number = a.reverse();
		newNum.normalize();
	}
	number = ans.reverse();
	if (negative && newNum != 0) {
		*this -= 1;
	}
	return *this;
}

BigSmoke& BigSmoke::operator%=(const BigSmoke& right) {
	BigSmoke temp = *this / right;
	temp *= right;
	*this -= temp;
	return *this;
}

BigSmoke BigSmoke::operator-() const{
	BigSmoke a;
	a.number = number;
	a.negative = !negative;
	return a;
}

BigSmoke BigSmoke::operator+() const{
	return *this;
}

BigSmoke operator+(const BigSmoke& left, const BigSmoke& right) {
	BigSmoke c("0");
	c += left;
	c += right;
	return c;
}

BigSmoke operator*(const BigSmoke& left, const BigSmoke& right) {
	BigSmoke c = 1;
	c *= left;
	c *= right;
	return c;
}

BigSmoke operator/(const BigSmoke& left, const BigSmoke& right) {
	BigSmoke c = 1;
	c *= left;
	c /= right;
	return c;
}

BigSmoke operator%(const BigSmoke& left, const BigSmoke& right) {
	BigSmoke c = 1;
	c *= left;
	c %= right;
	return c;
}

BigSmoke operator-(const BigSmoke& left, const BigSmoke& right) {
	BigSmoke c = 0;
	c += left;
	c -= right;
	return c;
}

bool BigSmoke::operator==(const BigSmoke& right) const {
	if (number == "0" && right.number == "0") {
		return true;
	}
	if (number == right.number && negative == right.negative) {
		return true;
	}
	return false;
}

bool BigSmoke::operator!=(const BigSmoke& right) const {
	if (*this == right) {
		return false;
	}
	return true;
}

bool BigSmoke::operator>(const BigSmoke& right) const {
	if (right.negative && !negative) {
		return true;
	}
	if (!right.negative && negative) {
		return false;
	}
	if (right.negative && negative) {
		int size1 = right.size();
		int size2 = size();
		if (size1 > size2) {
			return true;
		}
		else if (size1 < size2) {
			return false;
		}
		for (int i = size1 - 1; i >= 0; i--) {
			if (number[i] > right.number[i]) {
				return false;
			}
			else if (number[i] < right.number[i]) {
				return true;
			}
		}
		return false;
	}
	int size1 = right.size();
	int size2 = size();
	if (size1 > size2) {
		return false;
	}
	else if (size1 < size2) {
		return true;
	}
	for (int i = size1 - 1; i >= 0; i--) {
		if (number[i] > right.number[i]) {
			return true;
		}
		else if (number[i] < right.number[i]) {
			return false;
		}
	}
	return false;
}

bool BigSmoke::operator<(const BigSmoke& right) const {
	return right > * this;
}

bool BigSmoke::operator>=(const BigSmoke& right) const {
	return (*this > right) || (*this == right);
}

bool BigSmoke::operator<=(const BigSmoke& right) const {
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
