#pragma once
#include<vector>

/// Basic template stack class.
/// T needs to be copy-constructable
/// T needs to be default-constructable
template<class T>
class Stack
{
private:
	/// Storage container for stack's data. 
	std::vector<T> data; 

public:
	// let the compiler generate the default
	// constructors and destructor for us

	/// Insert an element on the top of stack. 
	/// \param val The value of the element to insert.
	void push(const T& val) { data.push_back(val); }

	/// Returns the value of the top element.
	/// \return Const reference to the top element.
	const T& top() const { return data.back(); }

	/// Returns the value of the top element and removes it from the stack.
	/// \return Copy of the top element.
	T pop()
	{
		T temp = data.back();
		data.pop_back();
		return temp;
	}

	/// Number of elements left on the stack. 
	/// \return The size of the container.
	size_t size() const { return data.size(); }
};