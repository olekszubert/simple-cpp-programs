#include <iostream>


int binarySearch(int* arr, int begin, int end, int value)
{
    if (end >= begin) //must check
    {
        int middle = (begin + end) / 2;

        if (value == arr[middle]) //value found
            return middle;

        if (value > arr[middle]) //greater
            return binarySearch(arr, middle + 1, end, value);

        if (value < arr[middle]) //lesser
            return binarySearch(arr, begin, middle - 1, value);
    }

    return -1;
}

int main()
{
    const int size = 100;
    int array[size] = { 0 };

    for (int i = 0; i < size; ++i)
        array[i] = i * 4;

    for (int i = 0; i < size; ++i)
        std::cout << array[i] << ", ";

    std::cout << "\n\n";

    int begin = 0;
    int end = sizeof(array) / sizeof(int) - 1;

    for (int a = 3; a < 300; a *= 2)
    {
        int index = binarySearch(array, begin, end, a);
        if (index != -1)
            std::cout << "Value found at index " << index << ". Check: " << array[index] << std::endl;
        else
            std::cout << "Value not found." << std::endl;

    }


}