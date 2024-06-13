#pragma once
#include "cstddef"
#include "stdexcept"
#include "cstring"
#include <compare>
template<typename CT, int DIR>
class VectorItt{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = CT;
	//signed interger type returned when subtracting one pointer from another, used because its capable of representing the size of the largest possible object
	using difference_type = std::ptrdiff_t;
	using pointer = CT*;
	using reference = CT&;
	

	VectorItt(CT* p);
	VectorItt();
	VectorItt(const VectorItt& other);
	CT& operator*() const;
	CT* operator ->() const;
	CT& operator[](size_t i) const;
	VectorItt& operator++();
	VectorItt& operator--();
	VectorItt operator ++(int);
	VectorItt operator --(int);
	VectorItt& operator +=(difference_type i);
	VectorItt operator+(difference_type i) const;
	VectorItt operator-(difference_type i) const;
	difference_type operator-(const VectorItt& other) const;


	//Original spaceship did not take into account for reverse iterations, modified.
	friend auto operator<=>(const VectorItt& lhs, const VectorItt& other) {
		if constexpr (DIR == -1) {
			return other.ptr <=> lhs.ptr;
		}
		else {
			return lhs.ptr <=> other.ptr;
		}
	};
	
	friend bool operator==(const VectorItt& lhs, const VectorItt& rhs) {
		return lhs.ptr == rhs.ptr;
	};

	/*
	* friend bool operator!=(const VectorItt& lhs, const VectorItt& rhs) {
		return lhs.elements != rhs.elements;
	};
	*/
	

	
private:
	CT* ptr;
};

//ClassName<TemplateParameters>:: syntax to tell compiler exactly which class's method im defininig. 
template<typename CT, int DIR>
VectorItt<CT, DIR>::VectorItt(CT* p) : ptr(p) {}

template<typename CT, int DIR>
VectorItt<CT, DIR>::VectorItt() : ptr(nullptr) {}

template<typename CT, int DIR>
VectorItt<CT, DIR>::VectorItt(const VectorItt& other) : ptr(other.ptr) {
}

template<typename CT, int DIR>
CT& VectorItt<CT, DIR>::operator*() const {
	return *ptr;
}

template<typename CT, int DIR>
CT* VectorItt<CT, DIR>::operator->() const {
	return ptr;
}

template<typename CT, int DIR>
CT& VectorItt<CT, DIR>::operator[](size_t i) const {
	if (DIR == -1) {
		//reverse iterator problems in tests, added this
		return *(ptr - i);
	}
	else {
		// normal
		return ptr[i];
	}
}

template<typename CT, int DIR>
VectorItt<CT, DIR>& VectorItt<CT, DIR>::operator++() {
	ptr += DIR;
	return *this;
}

template<typename CT, int DIR>
VectorItt<CT, DIR>& VectorItt<CT, DIR>::operator--() {
	ptr -= DIR;
	return *this;
}

template<typename CT, int DIR>
VectorItt<CT, DIR> VectorItt<CT, DIR>::operator++(int) {
	VectorItt<CT, DIR> temp = *this;
	ptr += DIR;
	return temp;
}

template<typename CT, int DIR>
VectorItt<CT, DIR> VectorItt<CT, DIR>::operator--(int) {
	VectorItt<CT, DIR> temp = *this;
	ptr -= DIR;
	return temp;
}


template<typename CT, int DIR>
VectorItt<CT, DIR>& VectorItt<CT, DIR>::operator+=(difference_type i) {
	ptr += i * DIR;
	return *this;
}

template<typename CT, int DIR>
VectorItt<CT, DIR> VectorItt<CT, DIR>::operator+(difference_type i) const {
	VectorItt temp = *this;
	temp += i; 
	return temp;
}

template<typename CT, int DIR>
VectorItt<CT, DIR> VectorItt<CT, DIR>::operator-(difference_type i) const {
	return *this + (-i);
}

template<typename CT, int DIR>
typename VectorItt<CT, DIR>::difference_type VectorItt<CT, DIR>::operator-(const VectorItt& other) const {
	return (ptr - other.ptr) * DIR;
}
