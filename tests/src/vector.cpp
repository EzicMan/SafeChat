#include "catch.hpp"
#include <algorithm>
#include <cstring>

#define VECTOR_CHECK_ALLOC_NUM
#include <Vector.h>

class CTestEx {
public:
	enum Constrs {
		ConstrDef = 0,
		ConstrCopy,
		ConstrMove,
		Destr,
		ConstrCount
	};

	static int m_sThrowAfter[ConstrCount];
	static int m_sTimesCalled[ConstrCount];

	CTestEx()
	{
		m_sTimesCalled[ConstrDef]++;

		if (m_sThrowAfter[ConstrDef] != -1) {
			m_sThrowAfter[ConstrDef]--;

			if (m_sThrowAfter[ConstrDef] == 0) {
				m_sThrowAfter[ConstrDef]--;
				throw std::runtime_error("CTestEx()");
			}
		}
	}

	CTestEx(const CTestEx&)
	{
		m_sTimesCalled[ConstrCopy]++;

		if (m_sThrowAfter[ConstrCopy] != -1) {
			m_sThrowAfter[ConstrCopy]--;

			if (m_sThrowAfter[ConstrCopy] == 0) {
				m_sThrowAfter[ConstrCopy]--;
				throw std::runtime_error("CTestEx(const CTestEx &)");
			}
		}
	}

	CTestEx(CTestEx&&) noexcept
	{
		m_sTimesCalled[ConstrMove]++;

		/*if (m_sThrowAfter[ConstrMove] != -1)
		{
			m_sThrowAfter[ConstrMove]--;

			if (m_sThrowAfter[ConstrMove] == 0) {
				m_sThrowAfter[ConstrMove]--;
				throw std::runtime_error("CTestEx(CTestEx &&)");
			}
		}*/
	}

	~CTestEx()
	{
		m_sTimesCalled[Destr]++;
	}

	static void Clear()
	{
		for (int i = 0; i < CTestEx::ConstrCount; i++)
			CTestEx::m_sThrowAfter[i] = -1;

		for (int i = 0; i < CTestEx::ConstrCount; i++)
			CTestEx::m_sTimesCalled[i] = 0;
	}

private:
};

int CTestEx::m_sThrowAfter[ConstrCount];
int CTestEx::m_sTimesCalled[ConstrCount];

TEST_CASE("Vector constructors", "[vector]")
{

	CTestEx::Clear();

	SECTION("empty vector")
	{
		Vector<int> v;
		REQUIRE(v.size() == 0);
	}

	SECTION("vector with 10 elements")
	{
		Vector<int> v(10);
		REQUIRE(v.size() == 10);

		for (int i : v) {
			REQUIRE(i == 0);
		}
	}

	SECTION("vector with 10 elements with value -100500")
	{
		Vector<int> v(10, -100500);
		REQUIRE(v.size() == 10);

		for (int i : v) {
			REQUIRE(i == -100500);
		}
	}

	SECTION("copy constructor")
	{
		Vector<int> v1(10, 123);
		REQUIRE(v1.size() == 10);

		Vector<int> v2(v1);
		REQUIRE(v1.size() == 10);
		REQUIRE(v2.size() == 10);

		for (int i : v2) {
			REQUIRE(i == 123);
		}
	}

	SECTION("move constructor")
	{
		Vector<int> v1(10, 123);
		REQUIRE(v1.size() == 10);

		Vector<int> v2(std::move(v1));
		REQUIRE(v1.size() == 0);
		REQUIRE(v2.size() == 10);

		for (int i : v2) {
			REQUIRE(i == 123);
		}
	}
}

TEST_CASE("Constructor exception handling", "[vector]")
{
	CTestEx::Clear();

	CTestEx::m_sThrowAfter[CTestEx::ConstrDef] = 3;
	CTestEx::m_sThrowAfter[CTestEx::ConstrCopy] = 3;

	SECTION("vector with 10 elements, exception on 3rd")
	{
		REQUIRE_THROWS_AS(Vector<CTestEx>(10), std::exception);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrDef] == 3);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrCopy] == 0);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrMove] == 0);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::Destr] == 2);
	}

	SECTION("vector with 10 elements with def value, exception on 3rd")
	{
		REQUIRE_THROWS_AS(Vector<CTestEx>(10, CTestEx()), std::exception);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrDef] == 1);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrCopy] == 3);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrMove] == 0);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::Destr] == 2 + 1);
	}

	SECTION("vector with 10 elements copy, exception on 3rd")
	{
		CTestEx::Clear();

		Vector<CTestEx> v(10);

		CTestEx::Clear();
		CTestEx::m_sThrowAfter[CTestEx::ConstrDef] = -1;
		CTestEx::m_sThrowAfter[CTestEx::ConstrCopy] = 3;

		REQUIRE_THROWS_AS(Vector<CTestEx>(v), std::exception);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrDef] == 0);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrCopy] == 3);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrMove] == 0);
		REQUIRE(CTestEx::m_sTimesCalled[CTestEx::Destr] == 2);
	}
}

TEST_CASE("Vector move assignment", "[vector]")
{
	CTestEx::Clear();
	Vector<CTestEx> v1(10);
	CTestEx::Clear();

	Vector<CTestEx> v2 = std::move(v1);
	REQUIRE(v1.size() == 0);
	REQUIRE(v2.size() == 10);
	REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrDef] == 0);
	REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrCopy] == 0);
	REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrMove] == 0);
	REQUIRE(CTestEx::m_sTimesCalled[CTestEx::Destr] == 0);
}

TEST_CASE("Vector::clear()", "[vector]")
{
	CTestEx::Clear();

	Vector<CTestEx> v(10);
	v.clear();

	REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrDef] == 10);
	REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrCopy] == 0);
	REQUIRE(CTestEx::m_sTimesCalled[CTestEx::ConstrMove] == 0);
	REQUIRE(CTestEx::m_sTimesCalled[CTestEx::Destr] == 10);
}

TEST_CASE("Vector::push_back()", "[vector]")
{
	Vector<int> v;

	for (size_t i = 1; i <= 10; i++) {
		v.push_back(i);
	}

	for (size_t i = 1; i <= 10; i++) {
		REQUIRE(v[i - 1] == i);
	}
}
