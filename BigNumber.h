#pragma once
#include <iostream>

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

String reverser(String a); // reverses the string

class BigSmoke {
	String number; // the number itself(CONTAINED BACKWARDS!)
	bool negative = false; // defines if number negative or not
public:
	//constructors
	BigSmoke();
	BigSmoke(const BigSmoke& a);
	BigSmoke(const long long a);
	String toString() const; //convert number to string and returns it
	const long long toNum() const; // converts number to long long and returns it
	const int size() const; // returns size of the number
	const bool znak() const; // returns true if number negative or false if positive
	void normalize(); // deletes all non-meaningfull zeros from the number
	// basic operators
	BigSmoke& operator+=(const BigSmoke& right);
	BigSmoke& operator*=(const BigSmoke& right);
	BigSmoke& operator-=(const BigSmoke& right);
	BigSmoke& operator/=(const BigSmoke& right);
	BigSmoke& operator%=(const BigSmoke& right);
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