#pragma once
#include <iostream>

long long Max(long long a, long long b);

class String {
	char* string; // string itself
	int sSize; // size of the string(without \0)
public:
	//constructors
	String();
	String(char* a);
	String(const char* a);
	String(char a);
	String(const String& a);
	~String(); //destructor
	const int size() const; // returns size of the string(without \0)
	const char* begin() const; // returns pointer to beginning of the string
	const char* end() const; // returns pointer to the end of the string
	const char* getCharAr() const; // converts string to char array and returns it
	String substring(int sindex, int eindex) const; // gets the substring [sindex;eindex)
	String reverse() const; // reverses the string
	//basic operators
	String& operator+=(const String& right);
	String& operator=(const String& right);
	bool operator==(const String& right) const;
	bool operator!=(const String& right) const;
	String& operator*=(int times);
	friend String operator*(const String& left, int times);
	friend String operator*(int times, const String& left);
	friend String operator+(const String& left, const String& right);
	char& operator[](int index) const;
	friend std::ostream& operator<<(std::ostream& os, const String& r);
	friend std::istream& operator>>(std::istream& is, String& r);
};

class BigSmoke {
	String number; // the number itself(CONTAINED BACKWARDS!)
	bool negative = false; // defines if number negative or not
public:
	//constructors
	BigSmoke();
	BigSmoke(const BigSmoke& a);
	BigSmoke(const String a);
	BigSmoke(const long long a);
	String toString() const; //convert number to string and returns it
	BigSmoke abs() const; //returns absolute value of number
	const int size() const; // returns size of the number
	const bool isNegative() const; // returns true if number negative or false if positive
	void normalize(); // deletes all non-meaningfull zeros from the number
	// basic operators
	BigSmoke& operator+=(const BigSmoke& right);
	BigSmoke& operator*=(const BigSmoke& right);
	BigSmoke& operator-=(const BigSmoke& right);
	BigSmoke& operator/=(const BigSmoke& right);
	BigSmoke& operator%=(const BigSmoke& right);
	BigSmoke operator-() const;
	BigSmoke operator+() const;
	friend BigSmoke operator+(const BigSmoke& left, const BigSmoke& right);
	friend BigSmoke operator*(const BigSmoke& left, const BigSmoke& right);
	friend BigSmoke operator/(const BigSmoke& left, const BigSmoke& right);
	friend BigSmoke operator%(const BigSmoke& left, const BigSmoke& right);
	friend BigSmoke operator-(const BigSmoke& left, const BigSmoke& right);
	bool operator==(const BigSmoke& right) const;
	bool operator!=(const BigSmoke& right) const;
	bool operator>(const BigSmoke& right) const;
	bool operator<(const BigSmoke& right) const;
	bool operator>=(const BigSmoke& right) const;
	bool operator<=(const BigSmoke& right) const;
	friend std::ostream& operator<<(std::ostream& os, const BigSmoke& r);
};