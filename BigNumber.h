#pragma once
#include <iostream>

long long Max(long long a, long long b);

//-----------------------------------------------------
// String
//-----------------------------------------------------
class String {
public:
	//constructors
	String();
	String(const char* a);
	String(char a);

	String(const String& a);
	String(String&& rhs) noexcept;

	//destructor
	~String();

	//assignment
	String& operator=(const String& right);
	String& operator=(String&& rhs) noexcept;

	long long size() const; // returns size of the string(without \0)
	const char* getCharAr() const; // converts string to char array and returns it
	char* getCharAr(); // converts string to char array and returns it

	const char* begin() const; // returns const iterator to beginning of the string
	const char* end() const; // returns const iterator to the end of the string
	char* begin(); // returns iterator to beginning of the string
	char* end(); // returns iterator to the end of the string

	String substring(long long sindex, long long eindex) const; // gets the substring [sindex;eindex)
	String reverse() const; // reverses the string
	String toLower() const; // returns string with all lower symbols
	String toUpper() const; // returns string with all higher symbols

	//basic operators
	String& operator+=(const String& right);
	bool operator==(const String& right) const;
	bool operator!=(const String& right) const;
	String& operator*=(long long times);
	friend String operator*(const String& left, long long times);
	friend String operator*(long long times, const String& left);
	friend String operator+(const String& left, const String& right);
	char& operator[](long long index) const;
	friend std::ostream& operator<<(std::ostream& os, const String& r);
	friend std::istream& operator>>(std::istream& is, String& r);

private:
	/**
	 * Dummy struct used as an argument for private constructor.
	 * @see String(DummyStruct)
	 */
	struct DummyStruct {
	};

	/**
	 * Points to `new char[]` allocated block of memory containing the string.
	 */
	char* string = nullptr;

	/**
	 * Length of the string.
	 */
	long long sSize = 0;

	/**
	 * Size of allocated memory excluding null terminator.
	 */
	long long m_iCapacity = 0;

	/**
	 * Uninitialized string constructor for internal optimizations.
	 */
	String(DummyStruct);

	/**
	 * Adjusts capacity to hold at least n chars.
	 */
	void _setCapacity(long long n);

	/**
	 * Adjusts capacity to hold exactly n chars.
	 */
	void _setExactCapacity(long long n);

	/**
	 * Frees all memory allocated by the string and leaves the string in unusable state.
	 */
	void _freeStringMem();

	/**
	 * Allocates memory chunk of size n.
	 */
	static char* _allocateMemBlock(long long n);

	/**
	 * Frees memory chunk allocated with _allocateMemBlock.
	 */
	static void _freeMemBlock(char* mem);

	/**
	 * Empty string. Must be all zeroes.
	 */
	static char m_sEmptyString[16];
};

inline long long String::size() const
{
	return sSize;
}

inline const char* String::getCharAr() const
{
	return string;
}

inline char* String::getCharAr()
{
	return string;
}

inline const char* String::begin() const
{
	return string;
}

inline const char* String::end() const
{
	return string + sSize;
}

inline char* String::begin()
{
	return string;
}

inline char* String::end()
{
	return string + sSize;
}

//-----------------------------------------------------
// BigSmoke
//-----------------------------------------------------
class BigSmoke {
	String number; // the number itself(CONTAINED BACKWARDS!)
	bool negative = false; // defines if number negative or not
public:
	//constructors
	BigSmoke();
	BigSmoke(const String a);
	BigSmoke(const long long a);
	String toString() const; //convert number to string and returns it
	String asHexString() const; // returns number as hex string
	BigSmoke abs() const; //returns absolute value of number
	const long long size() const; // returns size of the number
	const bool isNegative() const; // returns true if number negative or false if positive
	void normalize(); // deletes all non-meaningfull zeros from the number
	// basic operators
	BigSmoke& operator+=(const BigSmoke& right);
	BigSmoke& operator++();
	BigSmoke& operator--();
	BigSmoke operator++(int);
	BigSmoke operator--(int);
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