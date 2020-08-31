#include <iostream>
#include <vector> //for comparison
#include <string>
#include <chrono>
/*
A template vector class.

"Simple" - without optimizations of EmplaceBack and actual move.

TODO:
Iterators
Erase
*/

//Check if index is within range (debug only)
#define CHECK_INDEX if(!(index < m_Size)) __debugbreak()
#define NEW_CAPACITY m_Capacity + m_Capacity / 2 + 1

template<typename T>
class Vector
{
public:
	Vector() 
	{ 
		std::cout << "Vector Construct\n";
		Realloc(2); 
	}

	~Vector() {	}

	void PushBack(const T& value)
	{
		std::cout << "PushBack\n";

		if (m_Size >= m_Capacity)
			Realloc(NEW_CAPACITY);

		m_Data[m_Size++] = value;
	}

	void PushBack(T&& value)
	{
		std::cout << "PushBack(move)\n";

		if (m_Size >= m_Capacity)
			Realloc(NEW_CAPACITY);

		m_Data[m_Size++] = std::move(value);
	}

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
		//Calling type destructor manually for each element
		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		m_Size = 0;
	}

	//Variadic template
	//If we forward arguments not compatible with any constructor, we'll get a compiler error
	template<typename... Args>
	T& EmplaceBack(Args&&... args)
	{
		if (m_Size >= m_Capacity)
			Realloc(NEW_CAPACITY);

		m_Data[m_Size] = T(std::forward<Args>(args)...);

		//Placement new ...
		//new(&m_Data[m_Size]) T(std::forward<Args>(args)...);

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

	void Reserve(size_t newCapacity) 
	{
		std::cout << "Reserve called, new capacity: " << newCapacity << "\n";
		Realloc(newCapacity);
	}

private:
	void Realloc(size_t newCapacity)
	{
		std::cout << "Realloc called, new capacity: " << newCapacity << " ---\n";
		/*
		Allocate a new block of memory
		Copy all existing elements in vector to this new block of memory (TODO: move instead of copy)
		Delete old block of memory
		*/
		T* newBlock = new T[newCapacity];

		/*
		instead of T* newBlock = new T[newCapacity]; ...new calls constructors
		There shouldnt be any constructors here, we need to allocate enough memory.
		::operator in global namespace to make sure it works properly, returns void*, so cast to T*
		*/
		//T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

		//For shrinking
		if (newCapacity < m_Size)
			m_Size = newCapacity;

		//Dont use memcpy - for more complex types we need a copy constructor
		for (size_t i = 0; i < m_Size; ++i)
			newBlock[i] = m_Data[i];
			//newBlock[i] = std::move(m_Data[i]); //if there's no move constr/oper - will just copy

		//for (size_t i = 0; i < m_Size; i++)
		//	m_Data[i].~T();

		delete[] m_Data;
		//::operator delete(m_Data, m_Capacity * sizeof(T));
		m_Data = newBlock;
		m_Capacity = newCapacity;
		std::cout << "Realloc end ---\n";
	}

public:
	size_t Size() const { return m_Size; }
	size_t Capacity() const { return m_Capacity; }

private:
	T* m_Data = nullptr;

	size_t m_Capacity = 0;
	size_t m_Size = 0;
};


//---------------------------------------------------------------------------------------------------------------------------------

struct Vector3
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	Vector3() { std::cout << "Construct\n"; }
	Vector3(float scalar) : x(scalar), y(scalar), z(scalar) { std::cout << "Construct\n"; }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) { std::cout << "Construct\n"; }

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
		std::cout << "Copy (operator=)\n";
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vector3& operator=(Vector3&& other) noexcept
	{
		std::cout << "Move (operator=)\n";
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
};

//---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
void PrintVector(const Vector<T>& vector)
{
	for (size_t i = 0; i < vector.Size(); i++)
		std::cout << vector[i] << std::endl;

	std::cout << "DataSize: " << vector.Size() << "\n";
	std::cout << "Capacity: " << vector.Capacity() << "\n";
}

//Special for Vector3
template<>
void PrintVector(const Vector<Vector3>& vector)
{
	for (size_t i = 0; i < vector.Size(); i++)
		std::cout << vector[i].x << ", " << vector[i].y << ", " << vector[i].z << std::endl;

	std::cout << "DataSize: " << vector.Size() << "\n";
	std::cout << "Capacity: " << vector.Capacity() << "\n";
}

//Special for Vector
template<typename T>
void PrintVector(const Vector<Vector<T>>& vector)
{
	for (size_t i = 0; i < vector.Size(); i++)
	{
		for (size_t j = 0; j < vector[i].Size(); j++)
			std::cout << vector[i][j] << ", ";
		std::cout << std::endl;
	}

	std::cout << "DataSize: " << vector.Size() << "\n";
	std::cout << "Capacity: " << vector.Capacity() << "\n";
}

//---------------------------------------------------------------------------------------------------------------------------------

class Timer
{
public:
	Timer()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		Stop();
	}
	void Stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;

		std::cout << "\nTime: " << ms << "ms (" << duration << "us)\n\n";
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};

int main()
{
	{
		Timer timer;

		Vector<Vector3> vec1;

		auto tmp1 = Vector3(1.3f);
		auto tmp2 = Vector3(1.3f, 3.7f, 6.6f);

		vec1.PushBack(tmp1);
		vec1.PushBack(tmp2);
		vec1.PushBack(tmp1);
		vec1.PushBack(tmp2);
		PrintVector(vec1);
		std::cout << "-----------------------------------------Vec1 lvalue PushBack^\n";

		vec1.PushBack(Vector3(1.3f));
		vec1.PushBack(Vector3(1.3f, 3.7f, 6.6f));
		vec1.PushBack(Vector3(1.3f));
		vec1.PushBack(Vector3(1.3f, 3.7f, 6.6f));
		vec1.PushBack(Vector3(1.3f));
		vec1.PushBack(Vector3(1.3f, 3.7f, 6.6f));
		PrintVector(vec1);
		std::cout << "-----------------------------------------Vec1 rvalue PushBack^\n";

		vec1.PopBack();
		vec1.PopBack();
		PrintVector(vec1);
		std::cout << "-----------------------------------------Vec1 PopBack^\n";

		vec1.EmplaceBack(Vector3(1.3f));
		vec1.EmplaceBack(Vector3(1.3f, 3.7f, 6.6f));
		PrintVector(vec1);
		std::cout << "-----------------------------------------Vec1 rvalue EmplaceBack^\n";

		vec1.Clear();
		PrintVector(vec1);
		std::cout << "-----------------------------------------Vec1 Clear^\n";

		/*vec1.PushBack(Vector3(1.3f));
		vec1.PushBack(Vector3(1.3f, 3.7f, 6.6f));
		PrintVector(vec1);*/

		Vector<std::string> vec2;
		vec2.PushBack("Kek");
		vec2.PushBack("Meh");
		vec2.PushBack("Kek");
		vec2.PushBack("Meh");
		vec2.PushBack("Kek");
		vec2.PushBack("Meh");
		PrintVector(vec2);
		std::cout << "-----------------------------------------Vec2 PushBack std::string rvalue^\n";

		Vector<int> vec3;
		vec3.Reserve(8);
		vec3.PushBack(1);
		vec3.PushBack(2);
		vec3.PushBack(3);
		vec3.PushBack(4);
		vec3.PushBack(5);
		vec3.PushBack(6);
		vec3.PushBack(7);
		vec3.PushBack(8);
		PrintVector(vec3);
		std::cout << "-----------------------------------------Vec3 PushBack int (with reserve for 8)^\n";

		Vector<Vector<int>> vec4;
		vec4.PushBack(vec3);
		vec4.PushBack(vec3);
		vec4.PushBack(vec3);
		vec4.PushBack(vec3);
		vec4.PushBack(vec3);
		vec4.PushBack(vec3);
		PrintVector(vec4);
		std::cout << "-----------------------------------------Vec4 PushBack^\n";

		return 0;
	}
}
