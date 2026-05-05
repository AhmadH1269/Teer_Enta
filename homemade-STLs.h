#pragma once

// A dynamic array struct to support dynamically increasing during the runtime efficiently
template <typename Ty> struct Vector {
	int size;
	int capacity;
	Ty* data;

	Vector() {
		size = 0;
		capacity = 0;
		data = nullptr;
	}
	Vector(int size, const Ty& value = Ty{}) {
		this->size = size;
		capacity = size;
		data = new Ty[capacity];
		for (int i = 0; i < size; i++)
			data[i] = value;
	}
	Vector(const Vector<Ty>& other_vector) {
		size = other_vector.size;
		capacity = other_vector.capacity;
		data = new Ty[capacity];
		for (int i = 0; i < size; i++)
			data[i] = other_vector.data[i];
	}

	// An operator to make calling an element inside the array more convenient
	Ty& operator[] (const int i) const {
		return data[i];
	}

	Vector<Ty>& operator=(const Vector<Ty>& other_vector) {

		if (this == &other_vector)
			return *this;

		delete[] data;

		size = other_vector.size;
		capacity = other_vector.capacity;

		data = new Ty[capacity];

		for (int i = 0; i < size; i++)
			data[i] = other_vector.data[i];

		return *this;
	}

	bool operator==(const Vector<Ty>& other) const {
		if (size != other.size)
			return false;
		for (int i = 0; i < size; i++) {
			if (data[i] != other.data[i])
				return false;
		}
		return true;
	}

	void expand();
	void push_back(const Ty& x);
	void erase(int i);
	int find(Ty x);
	void reserve(unsigned int i);

	// A destructor to delete data when vector used goes out of scope (not used anymore)
	~Vector() {
		// To prevent memory leakage
		delete[] data;
	}
};

// Reallocating existing data in a larger dynamic array to be able to hold more data
template <typename Ty> void Vector<Ty>::expand() {
	if (!capacity) capacity = 1;		// Allocation size of the first element
	else capacity *= 2;					// Double the allocation size to be able to hold more elements
	Ty* new_data = new Ty[capacity];	// Allocate a new dynamic array with the new capacity

	// Allocate data in the new dynamic array
	for (int i = 0; i < size; i++)
		new_data[i] = data[i];

	delete[] data;						// delete the old array
	data = new_data;					// point the data at the new array
}
// Appending more data of the same type to the back of the array
template <typename Ty> void Vector<Ty>::push_back(const Ty& x) {
	if (size == capacity) expand();	// Expand the array if it's full

	data[size++] = x;				// Append the new element to the back of the array
}
// Erasing an element from any index inside of the array
template <typename Ty> void Vector<Ty>::erase(int in) {
	if (in < 0 || in >= size) return;	// Do nothing if the size is wrong
	// Shift elements after index to the left
	for (int i = in; i < size - 1; i++)
		data[i] = data[i + 1];
	size--;								// Update the size of the array
}

//
template <typename Ty> int Vector<Ty>::find(Ty x) {
	for (int i = 0; i < size; i++) {
		if (data[i] == x)
			return i;
	}

	return -1;
}

//
template <typename Ty> void Vector<Ty>::reserve(unsigned int i) {
	if (i < size) capacity = size;
	else capacity = i;
	Ty* new_data = new Ty[capacity];    // Allocate a new dynamic array with the new capacity

	// Allocate data in the new dynamic array
	for (int i = 0; i < size; i++)
		new_data[i] = data[i];

	delete[] data;                        // delete the old array
	data = new_data;                    // point the data at the new array
}


// A struct to hold any type of two objects together mimicking the C++ STL pair
template <typename Ty1, typename Ty2> struct Pair {
	Ty1 first;
	Ty2 second;
};