#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <type_traits>

// Define VECTOR_CHECK_ALLOC_NUM to enable internal leak check

template <typename T>
class Vector {
public:
	/*
	 * Constructs an empty vector
	 */
	Vector() {}

	/*
	 * Constructs a vector of size n
	 * @arg n Initial vector size
	 */
	Vector(size_t n) {
		try {
			setCapacity(n);
			for (size_t i = 0; i < n; i++)
			{
				new (getData() + i) T();
				m_iSize++;
			}
		}
		catch (...) {
			freeMem();
			throw;
		}
	}

	/*
	 * Constructs a vector of size n with def_val
	 * @arg n Initial vector size
	 * @arg def_val Value for those n elements
	 */
	Vector(size_t n, const T &def_val) {
		try {
			setCapacity(n);
			for (size_t i = 0; i < n; i++)
			{
				new (getData() + i) T(def_val);
				m_iSize++;
			}
		}
		catch (...) {
			freeMem();
			throw;
		}
	}

	/**
	 * Constructs a copy of vector "from"
	 * @arg from Vector to copy from
	 */
	Vector(const Vector &from) {
		try {
			setCapacity(from.size());
			for (size_t i = 0; i < from.size(); i++)
			{
				new (getData() + i) T(from[i]);
				m_iSize++;
			}
		}
		catch (...) {
			freeMem();
			throw;
		}
	}

	/**
	 * Moves vector
	 */
	Vector(Vector &&from) noexcept {
		moveFrom(std::move(from));
	}

	~Vector() {
		freeMem();
	}

	Vector &operator=(const Vector &) = delete;

	Vector &operator=(Vector &&from) noexcept
	{
		moveFrom(std::move(from));
		return *this;
	}

	//-----------------------------------------------------------

	/**
	 * Returns the number of elements in the vector
	 */
	inline size_t size() const {
		return m_iSize;
	}

	/**
	 * Returns capacity - maximum number of elements a vector can have before memory reallocation happens
	 */
	inline size_t capacity() const {
		return m_iCapaity;
	}

	/**
	 * Returns the pointer to the first element.
	 * If the vector is empty, behavior is undefined.
	 */
	inline T *getData() {
		return reinterpret_cast<T *>(m_pData);
	}

	/**
	 * Returns the pointer to the first element (const).
	 * If the vector is empty, behavior is undefined.
	 */
	inline const T *getData() const {
		return reinterpret_cast<const T *>(m_pData);
	}

	inline T *begin() {
		return getData();
	}

	inline T *end() {
		return getData() + m_iSize;
	}

	inline const T *begin() const {
		return getData();
	}

	inline const T *end() const {
		return getData() + m_iSize;
	}

	/**
	 * Returns reference to element at index idx
	 */
	inline T &operator[] (size_t idx) {
		assert(idx < m_iSize);
		return getData()[idx];
	}

	/**
	 * Returns const reference to element at index idx
	 */
	inline const T &operator[] (size_t idx) const {
		assert(idx < m_iSize);
		return getData()[idx];
	}

	//-----------------------------------------------------------

	/**
	 * Add an element to the end of the vector by copying it.
	 */
	void push_back(const T &obj) {
		extendCapacityIfNeedTo();
		new (getData() + m_iSize) T(obj);
		m_iSize++;
	}

	/**
	 * Add an element to the end of the vector by moving it.
	 */
	void push_back(T &&obj) {
		extendCapacityIfNeedTo();
		new (getData() + m_iSize) T(std::move(obj));
		m_iSize++;
	}

	/**
	 * Removes all elements from the vector.
	 * Capacity is not changed.
	 */
	void clear() {
		// Call destructors
		for (size_t i = 0; i < m_iSize; i++)
		{
			getData()[i].~T();
		}
		m_iSize = 0;
	}

private:
	uint8_t *m_pData = nullptr;
	size_t m_iSize = 0;
	size_t m_iCapaity = 0;

#ifdef VECTOR_CHECK_ALLOC_NUM
	size_t m_iAllocCount = 0;
#endif

	/**
	 * Extends the capacity to newCap. Doesn't decrease it.
	 */
	void setCapacity(size_t newCap) {
		if (m_iCapaity == 0)
		{
			// Allocate new array
			try {
				assert(!m_pData);
				m_pData = allocMemBlock(newCap);
				m_iCapaity = newCap;
			}
			catch (const std::bad_alloc &) {
				throw;
			}
		}
		else
		{
			// Extend the capacity
			uint8_t *newData = nullptr;

			// Try to allocate new array
			try {
				newData = allocMemBlock(newCap);
			}
			catch (const std::bad_alloc &) {
				throw;
			}

			T *newDataT = reinterpret_cast<T *>(newData);

			// Move objects from m_pData to newData
			try {
				for (size_t i = 0; i < m_iSize; i++)
				{
					// Move T from m_pData[i] to newData[i]
					new (newDataT + i) T(std::move(getData()[i]));
				}
			}
			catch (...) {
				// A move constructor has thrown an exception.
				// It is essentially impossible to restore integrity of the vector.
				std::abort();
			}

			// Destroy the old array
			for (size_t i = 0; i < m_iSize; i++)
			{
				getData()[i].~T();
			}
			freeMemBlock(m_pData);

			m_pData = newData;
			m_iCapaity = newCap;
		}
	}

	/**
	 * Extends the capacity to hold at least one more element
	 */
	void extendCapacityIfNeedTo() {
		if (m_iSize + 1 > m_iCapaity)
		{
			if (m_iCapaity == 0)
				setCapacity(1);
			else
				setCapacity(m_iCapaity * 2);
		}
	}

	/**
	 * Frees all allocated memory
	 */
	void freeMem() {
		if (m_pData) {
			assert(m_iCapaity > 0);
			clear();
			m_iCapaity = 0;
			freeMemBlock(m_pData);
		}

#ifdef VECTOR_CHECK_ALLOC_NUM
		assert(m_iAllocCount == 0);
#endif
	}

	/**
	 * Allocates a chunk of raw memory big enough to hold n Ts
	 * Must be freed with freeMemBlock
	 */
	uint8_t *allocMemBlock(size_t n) {
#ifdef VECTOR_CHECK_ALLOC_NUM
		m_iAllocCount++;
#endif
		return new uint8_t[n * sizeof(T)];
	}

	/**
	 * Frees memory allocated by allocMemBlock
	 */
	void freeMemBlock(uint8_t *block) {
		delete[] block;
#ifdef VECTOR_CHECK_ALLOC_NUM
		m_iAllocCount--;
#endif
	}

	void moveFrom(Vector &&from) {
#ifdef VECTOR_CHECK_ALLOC_NUM
		assert(from.m_iAllocCount == 1);
		m_iAllocCount = 1;
		from.m_iAllocCount = 0;
#endif
		m_pData = from.m_pData;
		m_iSize = from.m_iSize;
		m_iCapaity = from.m_iCapaity;

		from.m_pData = nullptr;
		from.m_iSize = 0;
		from.m_iCapaity = 0;
	}
};