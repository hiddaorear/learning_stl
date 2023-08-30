#include <iostream>
#include "stl_algo.h"

int main() {
    int arr[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
    std::vector<int> ivec(arr, arr + 9);

    my_make_heap(ivec.begin(), ivec.end());
    for (int i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << ' ';
    }
    std::cout << std::endl; // 9 5 8 3 4 0 2 3 1

    ivec.push_back(7);
    my_push_heap(ivec.begin(), ivec.end());
    for (int i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << ' ';
    }
    std::cout << std::endl; // 9 7 8 3 5 0 2 3 1 4

    my_pop_heap(ivec.begin(), ivec.end());
    std::cout << ivec.back() << std::endl; // 9
    ivec.pop_back();

    for(int i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << ' ';
    }
    std::cout << std::endl; // 8 7 4 3 5 0 2 3 1

    my_sort_heap(ivec.begin(), ivec.end());
    for (int i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << ' ';
    }
    std::cout << std::endl; // 0 1 2 3 3 4 5 7 8

    return 0;
}
