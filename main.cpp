#include <iostream>
#include "stl_algo.h"


int main() {
    int arr[] = {19, 93, 3, 25, 89, 6, 26};
    int len = (int) sizeof(arr) / sizeof(*arr);
    sort(arr, &arr[len]);
    for (int i = 0; i < len; i++) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
    return 0;
}
