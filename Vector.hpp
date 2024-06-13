#pragma once
#include "cstddef"
#include "stdexcept"
#include "cstring"
#include "VectorItt.hpp"
#include "Dalloc.hpp"

template<typename T>
class Vector {
private:
	size_t allocated_capacity;
	T* elements;
	size_t m_size;
	Dalloc<T> _dAlloc;
	
public:
	using iterator = VectorItt<T, +1>;
	using const_iterator = VectorItt<const T, +1>;
	using reverse_iterator = VectorItt<T, -1>;
	using const_reverse_iterator = VectorItt<const T, -1>;
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	
	Vector() noexcept : allocated_capacity(0), elements(nullptr), m_size(0) {};
	void clear() noexcept;
	//destruktor gör deallocate istället för delete
	~Vector() noexcept {
		/*
		for(size_t i = 0; i < m_size; ++i) {
			elements[i].~T();
		}
		*/
		clear();
		_dAlloc.deallocate(elements, allocated_capacity);
	};
	Vector(const char* other);
	Vector(const Vector& other);
	Vector(Vector&& other) noexcept;
	Vector& operator=(const Vector& other);
	Vector& operator=(Vector&& other) noexcept;
	const T& operator[](size_t i)const;
	T& operator[](size_t i);
	const T& at(size_t i)const;
	T& at(size_t i);
	const T* data()const noexcept;
	T* data() noexcept;
	size_t size() const noexcept;
	size_t capacity() const noexcept;
	void reserve(size_t n);
	void shrink_to_fit();
	void resize(size_t n);
	void push_back(const T& c);
	void pop_back();
	template<typename Iterator>
	Vector(Iterator begin, Iterator end);
	
	template<class... Args>
	void emplace_back(Args&&... args);

	
	friend bool operator==(const Vector& lhs, const Vector& other) {
		if (lhs.m_size != other.m_size) {
			return false;
		}
		for (size_t i = 0; i < lhs.m_size; ++i) {
			if (lhs.elements[i] != other.elements[i]) {
				return false;
			}
		}
		return true;
	};

	friend bool operator !=(const Vector<T>& lhs, const Vector<T>& other) {
		return !(lhs == other);
	};

	friend bool operator<(const Vector<T>& lhs, const Vector<T>& other) {
		// Compare element by element
		for (size_t i = 0; i < lhs.size(); ++i) {
			if (i >= other.size()) {
				return false;
			}
			if (lhs[i] < other[i]) {
				return true;
			}
			else if (lhs[i] > other[i]) {
				return false;
			}
		}
		return lhs.size() < other.size();
	};

	friend std::ostream& operator<<(std::ostream& cout, const Vector& other) {
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	};

	friend bool operator>(const Vector<T>& lhs, const Vector<T>& other) {
		return other < lhs;
	};

	friend bool operator<=(const Vector<T>& lhs, const Vector<T>& other) {
		return !(lhs > other);
	};

	friend bool operator>=(const Vector<T>& lhs, const Vector<T>& other) {
		return !(lhs < other);
	};
	//kanske inte ska vara här
	friend void swap(Vector<T>& lhs, Vector<T>& rhs) {
		using std::swap; // Enable ADL (Argument-Dependent Lookup) matches the correct swap function
		swap(lhs.allocated_capacity, rhs.allocated_capacity);
		swap(lhs.elements, rhs.elements);
		swap(lhs.m_size, rhs.m_size);
	};


	bool Invariant() const;
	
	
	iterator begin() noexcept {
		return iterator(elements);
	}

	//return iterator pointing one past last element in container
	
	iterator end() noexcept {
		return iterator(elements + m_size);
	}

	const_iterator begin() const noexcept {
		return const_iterator(elements);
	}

	
	const_iterator end() const noexcept {
		return const_iterator(elements + m_size);
	}

	
	const_iterator cbegin() const noexcept {
		return const_iterator(elements);
	}

	
	const_iterator cend() const noexcept {
		return const_iterator(elements + m_size);
	}

	
	reverse_iterator rbegin() noexcept {
		return m_size > 0 ? reverse_iterator(elements + m_size - 1) : reverse_iterator(elements);
	}

	reverse_iterator rend() noexcept {
		return reverse_iterator(elements - 1); // points to one before the first element
	}

	const_reverse_iterator rbegin() const noexcept {
		return m_size > 0 ? const_reverse_iterator(elements + m_size - 1) : const_reverse_iterator(elements);
	}

	const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator(elements - 1); // points to one before the first element
	}


	const_reverse_iterator crbegin() const noexcept {
		return m_size > 0 ? const_reverse_iterator(elements + m_size - 1) : const_reverse_iterator(elements);
	}

	const_reverse_iterator crend() const noexcept {
		return const_reverse_iterator(elements - 1); // points to one before the first element
	}
};
	

	/*
	template<typename T>
	template<typename Iterator>
	Vector<T>::Vector(Iterator begin, Iterator end) {
		size_t newCapacity = std::distance(begin, end);
		elements = _dAlloc.allocate(newCapacity); // Allocate memory for newCapacity elements
		size_t i = 0;
		try {
			for (auto it = begin; it != end; ++it, ++i) {
				new (elements + i) T(*it); // Using placement new to construct elements
			}
		}
		catch (...) {
			// If an exception is thrown, destroy any constructed elements
			for (size_t j = 0; j < i; ++j) {
				elements[j].~T();
			}
			_dAlloc.deallocate(elements, newCapacity); // Clean up allocated memory
			throw; // Rethrow the exception
		}
		allocated_capacity = newCapacity;
		m_size = newCapacity;
	}
	
	*/

	
	//constructor takes c style string
	//try catch for the strong exception 
	template<typename T>
	Vector<T>::Vector(const char* other) : allocated_capacity(0), elements(nullptr), m_size(0) {
		if (other == nullptr || *other == '\0') {
			// If nullptr or empty string, members are already initialized to handle this case.
		}
		else {
			size_t newSize = std::strlen(other);
			if (newSize > 0) {
				T* tempElements = _dAlloc.allocate(newSize); // Temporarily allocate space for newSize elements
				try {
					for (size_t i = 0; i < newSize; ++i) {
						// Use placement new for construction and handle exceptions
						new(tempElements + i) T(other[i]);
					}
					// If all constructions succeeded, assign the temporary array to the member variable
					elements = tempElements;
					m_size = newSize;
					allocated_capacity = newSize;
				}
				catch (...) {
					// If an exception is thrown, clean up already constructed elements
					for (size_t i = 0; i < newSize; ++i) {
						// Call destructor for each constructed element
						(tempElements + i)->~T();
					}
					// Free allocated memory
					_dAlloc.deallocate(tempElements, newSize);
					// Rethrow the exception to signal failure to construct the Vector
					throw;
				}
			}
		}
	}

	//copy constructor
	template<typename T>
	Vector<T>::Vector(const Vector& other)
		: allocated_capacity(other.m_size), m_size(0), elements(_dAlloc.allocate(other.m_size)) {
		if (other.m_size <= 0) { 
			return;
		}
		try {
			for (; m_size < other.m_size; ++m_size) {
				new (elements + m_size) T(other.elements[m_size]); // Use placement new
			}
		}
		catch (...) {
			// Cleadn up in case of an exception
			while (m_size > 0) {
				--m_size;
				elements[m_size].~T(); // Manually call the destructor!!
			}
			_dAlloc.deallocate(elements, allocated_capacity); // Deallocate the allocated memory /& clean up
			throw; // Rethrow the exception
		}
	}

	//move constructor
	//moving the vector "others" content to a new vector
	template<typename T>
	Vector<T>::Vector(Vector&& other) noexcept : allocated_capacity(other.allocated_capacity), elements(other.elements), m_size(other.m_size) {
		other.elements = nullptr;
		other.m_size = 0;
		other.allocated_capacity = 0;
	}



	//lvalue reference , non-temporary object, an object that occupies some identifiable location in memory,
	//either successfully updates or remain unchanged, strong
	//copy assignment operator
	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector& other) {
		if (this != &other) { // Check for self-assignment
			// Directly allocate new memory and construct copies of the other's elements.
			T* new_elements = _dAlloc.allocate(other.m_size);
			size_t i = 0;
			try {
				for (; i < other.m_size; ++i) {
					// Use placement new to construct elements
					new (new_elements + i) T(other.elements[i]);
				}
			}
			catch (...) {
				// If an exception is thrown, destroy any constructed elements
				for (size_t j = 0; j < i; ++j) {
					new_elements[j].~T();
				}
				// Deallocate the allocated memory
				_dAlloc.deallocate(new_elements, other.m_size);
				throw; // Rethrow the exception
			}

			//construction was successful , now destroy and deallocate the old elements
			for (size_t k = 0; k < m_size; ++k) {
				elements[k].~T();
			}
			_dAlloc.deallocate(elements, allocated_capacity);

			// Update the vector's state to the new state
			elements = new_elements;
			m_size = other.m_size;
			allocated_capacity = other.m_size; // Assume we match capacity to size for simplicity
		}
		return *this;
	}

	template<typename T>
	void Vector<T>::clear() noexcept {
		for (size_t i = 0; i < m_size; ++i) {
			elements[i].~T();
		}
		m_size = 0;
	}

	//rvalue reference to temporary object, or object about to be destroyed.
	//move 
	//self check needed because of the move semantics and the fact that the object is about to be destroyed
	//prevent deallocating its own resources inadvertently. 
	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
		if (this != &other) {
			// Call the destructor for each element in the destination vector
			for (size_t i = 0; i < m_size; ++i) {
				elements[i].~T();
			}
			_dAlloc.deallocate(elements, allocated_capacity);
			elements = other.elements;
			allocated_capacity = other.allocated_capacity;
			m_size = other.m_size;

			// Reset the source vector to a valid, empty state
			other.elements = nullptr;
			other.allocated_capacity = 0;
			other.m_size = 0;
		}
		return *this;
	}
	//kanske ändra
	template<typename T>
	const T& Vector<T>::operator[](size_t i)const {
			return elements[i];
	}


	//Strong because it guarantees that either operation successfully without altering the program state, or throws exception
	//const so doesnt modify state only access it, rollback / commit doesnt apply in the same sense
	template<typename T>
	const T& Vector<T>::at(size_t i)const {
		if (i < m_size) {
			return elements[i];
		}
		else {
			throw std::out_of_range("index out of range");
		}
	}
	

	template<typename T>
	T& Vector<T>::at(size_t i) {
		if (i < m_size) {
			return elements[i];
		}
		else {
			throw std::out_of_range("index out of range");
		}
	}

	
	template<typename T>
	T& Vector<T>::operator[](size_t i) {
		return elements[i];
	}
	
	template<typename T>
	const T* Vector<T>::data()const noexcept {
		return elements;
	}

	template<typename T>
	T* Vector<T>::data() noexcept {
		return elements;
	}

	template<typename T>
	size_t Vector<T>::size() const noexcept {
		return m_size;
	}

	template<typename T>
	size_t Vector<T>::capacity() const noexcept {
		return allocated_capacity;
	}

	//strong exception safety 
	//increases the vector capacity to at least n elements
	template<typename T>
	void Vector<T>::reserve(size_t n) {
		if (n <= allocated_capacity) {
			return; // no need if true
		}
		T* new_elements = _dAlloc.allocate(n); // allocate the new block
		size_t successfullyConstructed = 0; // how many elements successfully constructred 
		try {
			for (size_t i = 0; i < m_size; ++i) {
				//new to move contrsct T objects into the new memory block
				new(new_elements + i) T(std::move(elements[i]));
				successfullyConstructed++;
			}
		}
		catch (...) {
			// If an exception is thrown, destroy any successfully constructed elements.
			for (size_t i = 0; i < successfullyConstructed; ++i) {
				new_elements[i].~T(); //calls its destructor
			}
			// Deallocate the new memory block.
			_dAlloc.deallocate(new_elements, n);
			// Rethrow the exception to signal failure.
			throw;
		}
		// Destroy and deallocate old elements only after successful construction of new elements.
		for (size_t i = 0; i < m_size; ++i) {
			elements[i].~T();
		}
		_dAlloc.deallocate(elements, allocated_capacity);

		//update vector members to point to the new block of memory
		elements = new_elements;
		allocated_capacity = n;
	}

	//Strong try catch
	//reduces vector capacity t math its size by allocating new memory that fits current cumber of elements exactly
	//coopying elements to new memory block the ndeallocating the old memory, nsuring vector does not use more memory that necessary

	template<typename T>
	void Vector<T>::shrink_to_fit() {
		if (m_size == allocated_capacity) {
			return; // No operation needed if size equals capacity.
		}

		T* new_elements = _dAlloc.allocate(m_size); // Allocate memory for the new smaller array.
		size_t constructed = 0; // Track the number of successfully constructed objects in new_elements.

		try {
			// Attempt to copy elements to the new storage.
			for (size_t i = 0; i < m_size; ++i) {
				new (new_elements + i) T(elements[i]);
				++constructed; // Increment after successful construction.
			}
		}
		catch (...) {
			// If an exception is thrown during copying, destroy any constructed objects in new_elements.
			for (size_t i = 0; i < constructed; ++i) {
				new_elements[i].~T();
			}
			_dAlloc.deallocate(new_elements, m_size); // Deallocate new_elements since it won't be used.
			throw; // Rethrow the exception to signal failure.
		}

		// Destroy existing elements in the old storage, now that we've successfully copied them.
		for (size_t i = 0; i < m_size; ++i) {
			elements[i].~T();
		}

		// Deallocate old storage.
		_dAlloc.deallocate(elements, allocated_capacity);

		// Update the Vector's storage to the new smaller array and its capacity.
		elements = new_elements;
		allocated_capacity = m_size;
	}



	//adjust vector size to n elements, increasing capacity if necessary, adding defauly constructed elements if new size is greater than current size
	// or destoryuing excess elements if the new size is smaller, ensuring vector size matches n
	//abasic exception safety
	template<typename T>
	void Vector<T>::resize(size_t n) {
		if (n > allocated_capacity) {
			reserve(n); //  reserve provides strong exception safety
		}
		if (n > m_size) {
			// Construct new elements. If T's constructor throws, the vector's size remains at m_size.
			while (m_size < n) {
				new (elements + m_size) T();
				++m_size; // Only increment m_size after successful construction
			}
		}
		else {
			while (m_size > n) {
				--m_size; // Decrease m_size before destruction
				elements[m_size].~T(); // Destroy surplus elements
			}
		}
	}

	template<typename T>
	template<class... Args>
	void Vector<T>::emplace_back(Args&&... args) {
		if (m_size == allocated_capacity) {
			reserve(allocated_capacity == 0 ? 1 : 2 * allocated_capacity);
		}
		new(elements + m_size) T(std::forward<Args>(args)...);
		++m_size;
	}
	

	template<typename T>
	void Vector<T>::push_back(const T& c) {
		if (m_size == allocated_capacity) {
			// Attempt to reserve more space.
			reserve(allocated_capacity == 0 ? 1 : 2 * allocated_capacity);
		}
		//temporary copy of c so the construction doesnt throw
		//temporary object will be in the scope where its construction wont affect vector if thrown.
		// the exception would be thrown before altering the state of the vector, maintaining strong exception safety. 
		T temp(c);

		//place copy into array, noexcept  just moving full object which should not trow
		//new construct the new object directly into the memory location at elements + m_size
		new (elements + m_size) T(std::move(temp));

		// Increment m_size only after the non-throwing operation is guaranteed to succeed.
		++m_size;
	}


	template<typename T>
	void Vector<T>::pop_back() {
		if (m_size > 0) {
			m_size--;
		}
	}

	template<typename T>
	bool Vector<T>::Invariant() const {
		
		if (m_size > allocated_capacity) {
			return false;
		}

		
		if (m_size > 0 && elements == nullptr) {
			return false;
		}

		return true;  
	}


template class Vector<char>;

