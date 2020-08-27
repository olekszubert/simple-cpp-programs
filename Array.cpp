#include <iostream>
#include <array> //for comparison
/*
A template array class.
*/

//Check if index is within range (debug only)
#define CHECK_INDEX if(!(index < S)) __debugbreak()

namespace snk {

	template<typename T, size_t S>
	class Array
	{
	public:
		constexpr size_t Size() const { return S; }

		/*
		T& - return reference, because returning by value makes a copy (so for example copy of a string), 
		but we only want a read-only reference. Also, assigning is impossible because its not a modifiable lvalue.
		We still need a version of this for const reference if array happens to be const, otherwise we can't access it.
		*/
		T& operator[](size_t index) 
		{ 
		#ifdef _DEBUG
			CHECK_INDEX;
		#endif
			return m_Data[index]; 
		}

		/*
		Version for const array or const reference.
		*/
		const T& operator[](size_t index) const 
		{ 
		#ifdef _DEBUG
			CHECK_INDEX;
		#endif
			return m_Data[index]; 
		}

		/*
		A way to access the data within class
		Allows you to call memset
		*/
		T* Data() { return m_Data; }
		const T* Data() const { return m_Data; }


	private:
		T m_Data[S];
	};

}

int main()
{
	snk::Array<int, 10> arr1;

	memset(arr1.Data(), 0, arr1.Size() * sizeof(int));
	//or
	memset(&arr1[0], 0, arr1.Size() * sizeof(int));

	arr1[1] = 4;
	arr1[4] = 5;

	for (size_t i = 0; i < arr1.Size(); ++i)
	{
		std::cout << arr1[i] << std::endl;
	}

	std::cout << std::endl;

	snk::Array<std::string, 2> arr2;
	arr2[0] = "Kek";
	arr2[1] = "Meh";

	for (size_t i = 0; i < arr2.Size(); ++i)
	{
		std::cout << arr2[i] << std::endl;
	}

	//IMPLEMENT THIS
	//snk::Array<float, 3> arr3 = { 1.1f, 2.2f, 3.3f };

	/* IMPLEMENT ITERATORS
	for (auto& element : arr1)
	{

	}*/

	return 0;
}