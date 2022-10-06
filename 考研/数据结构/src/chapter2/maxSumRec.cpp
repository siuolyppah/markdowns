#include <iostream>
#include <algorithm>

int maxSumRec(int a[], int left, int right)
{
    if (left == right)
    {
        return a[left] > 0 ? a[left] : 0;
    }

    int center = (left + right) / 2;
    int maxLeftSum = maxSumRec(a, left, center);
    int maxRightSum = maxSumRec(a, center + 1, right);

    int maxLeftBorderSum = 0, leftBorderSum = 0;
    for (int i = center; i >= left; --i)
    {
        leftBorderSum += a[i];
        maxLeftBorderSum = std::max(maxLeftBorderSum, leftBorderSum);
    }

    int maxRightBorderSum = 0, rightBorderSum = 0;
    for (int i = center + 1; i <= right; ++i)
    {
        rightBorderSum += a[i];
        maxRightBorderSum = std::max(maxRightBorderSum, rightBorderSum);
    }

    return std::max<int>({maxLeftSum, maxRightSum, maxLeftBorderSum + maxRightBorderSum});
}



int main()
{
    constexpr int a_size = 8;
    int a[a_size] = {4, -3, 5, -2, -1, 2, 6, -3};

    int res = maxSumRec(a, 0, a_size - 1);

    std::cout << res << std::endl;

    return 0;
}