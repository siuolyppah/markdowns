#include <iostream>

template <typename T>
int binarySearch(T a[], T key, int size)
{
    int low = 0, high = size;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        if (a[mid] < key)
        {
            low = mid + 1;
        }
        else if (a[mid] > key)
        {
            high = mid - 1;
        }
        else
        {
            return mid;
        }
    }

    return -1;
}

int main()
{
    const int size = 8;
    int a[size] = {0, 1, 2, 3, 4, 5, 6, 7};

    std::cout << binarySearch(a, 6, size - 1) << std::endl;

    return 0;
}