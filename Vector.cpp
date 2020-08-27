#include <iostream>
#include <vector> //for comparison
#include <string>
/*
A template vector class.

TODO:
Iterators
Reserve
Erase
*/

//Check if index is within range (debug only)
#define CHECK_INDEX if(!(index < m_Size)) __debugbreak()

namespace snk {

	template<typename T>
	class Vector
	{
	public:
		Vector()
		{
			Realloc(2);
		}

		~Vector()
		{
			Clear();
			::operator delete(m_Data, m_Capacity * sizeof(T));
		}

		void PushBack(const T& value)
		{
			if (m_Size >= m_Capacity)
				Realloc(m_Capacity + m_Capacity / 2);

			m_Data[m_Size++] = value;
		}

		void PushBack(T&& value)
		{
			if (m_Size >= m_Capacity)
				Realloc(m_Capacity + m_Capacity / 2);

			m_Data[m_Size++] = std::move(value);
		}

		//Variadic template
		//If we forward arguments not compatible with any constructor, we'll get a compiler error
		template<typename... Args>
		T& EmplaceBack(Args&&... args)
		{
			if (m_Size >= m_Capacity)
				Realloc(m_Capacity + m_Capacity / 2);

			//m_Data[m_Size] = T(std::forward<Args>(args)...);

			//Placement new ...
			new(&m_Data[m_Size]) T(std::forward<Args>(args)...);

			return m_Data[m_Size++];
		}

		T& operator[](size_t index) 
		{ 
		#ifdef _DEBUG
			CHECK_INDEX;
		#endif
			return m_Data[index]; 
		}
		const T& operator[](size_t index) const	
		{ 
		#ifdef _DEBUG
			CHECK_INDEX;
		#endif
			return m_Data[index]; 
		}

		size_t Size() const { return m_Size; }

		void PopBack()
		{
			if (m_Size > 0)
			{
				m_Size--;
				m_Data[m_Size].~T();
			}
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~T();

			m_Size = 0;
		}

		void Reserve(T num){}

	private:
		void Realloc(size_t newCapacity)
		{
			//Allocate a new block of memory
			//Copy all existing elements in vector to this new block of memory (TODO: move instead of copy)
			//Delete old block of memory

			/*
			instead of T* newBlock = new T[newCapacity];
			There shouldnt be any constructors here, we need to allocate enough memory.
			::operator in global namespace to make sure it works properly, returns void*, so cast to T* 

			*/
			T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

			//For shrinking
			if (newCapacity < m_Size)
				m_Size = newCapacity;

			//Dont use memcpy - for more complex types we need a copy constructor
			for (size_t i = 0; i < m_Size; ++i)
				newBlock[i] = std::move(m_Data[i]); //if there's no move constr/oper - will just copy

			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~T();

			//delete[] m_Data;
			::operator delete(m_Data, m_Capacity * sizeof(T));
			m_Data = newBlock;
			m_Capacity = newCapacity;

		}

	private:
		T* m_Data = nullptr;

		int m_Capacity = 0;
		int m_Size = 0;
	};

}

struct Vector3
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	Vector3() {}
	Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	//Copy constructor
	Vector3(const Vector3& other)
		: x(other.x), y(other.y), z(other.z)
	{
		std::cout << "Copy\n";
	}

	//Move constructor
	Vector3(Vector3&& other) noexcept
		: x(other.x), y(other.y), z(other.z)
	{
		std::cout << "Move\n";
	}

	~Vector3()
	{
		std::cout << "Destroy\n";
	}

	Vector3& operator=(const Vector3& other)
	{
		std::cout << "Copy\n";
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vector3& operator=(Vector3&& other) noexcept
	{
		std::cout << "Move\n";
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
};

template<typename T>
void PrintVector(const snk::Vector<T>& vector)
{
	for (size_t i = 0; i < vector.Size(); i++)
		std::cout << vector[i] << std::endl;

	std::cout << std::endl;
}

//Special for Vector3
template<>
void PrintVector(const snk::Vector<Vector3>& vector)
{
	for (size_t i = 0; i < vector.Size(); i++)
		std::cout << vector[i].x << ", " << vector[i].y << ", " << vector[i].z << std::endl;

	std::cout << "\n\n";
}

int main()
{
	{
		snk::Vector<Vector3> vec1;

		vec1.EmplaceBack(Vector3(1.3f));
		vec1.EmplaceBack(Vector3(1.3f, 3.7f, 6.6f));
		vec1.EmplaceBack(Vector3(1.3f));
		vec1.EmplaceBack(Vector3(1.3f, 3.7f, 6.6f));
		PrintVector(vec1);

		vec1.PopBack();
		vec1.PopBack();
		PrintVector(vec1);

		vec1.PushBack(Vector3(1.3f));
		vec1.PushBack(Vector3(1.3f, 3.7f, 6.6f));
		PrintVector(vec1);

		vec1.Clear();
		PrintVector(vec1);

		vec1.EmplaceBack(Vector3(1.3f));
		vec1.EmplaceBack(Vector3(1.3f, 3.7f, 6.6f));
		PrintVector(vec1);


		snk::Vector<int> vec2;
		vec2.PushBack(5);
		vec2.EmplaceBack(4);
		vec2.EmplaceBack(4);
		vec2.EmplaceBack(4);
		vec2.EmplaceBack(4);
		PrintVector(vec2);

		vec2.PopBack();
		PrintVector(vec2);

		vec2.Clear();
		PrintVector(vec2);


		/*snk::Vector<std::string> vec3;
		vec3.PushBack("Kek");
		vec3.PushBack("Meh");
		vec3.EmplaceBack("Kek");
		vec3.EmplaceBack("Meh");
		PrintVector(vec3);

		vec3.PopBack();
		PrintVector(vec3);

		vec3.Clear();
		PrintVector(vec3);*/

		return 0;
	}
}
