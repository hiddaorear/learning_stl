#include <iostream>
#include "stl_algo.h"

int main() {
    int arr[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
    std::vector<int> ivec(arr, arr + 9);

    SGI::make_heap(ivec.begin(), ivec.end());
    for (int i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << ' ';
    }
    std::cout << std::endl; // 9 5 8 3 4 0 2 3 1

    ivec.push_back(7);
    SGI::push_heap(ivec.begin(), ivec.end());
    for (int i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << ' ';
    }
    std::cout << std::endl; // 9 7 8 3 5 0 2 3 1 4

    SGI::pop_heap(ivec.begin(), ivec.end());
    std::cout << ivec.back() << std::endl; // 9
    ivec.pop_back();

    for(int i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << ' ';
    }
    std::cout << std::endl; // 8 7 4 3 5 0 2 3 1

    SGI::sort_heap(ivec.begin(), ivec.end());
    for (int i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << ' ';
    }
    std::cout << std::endl; // 0 1 2 3 3 4 5 7 8

    int marge_sort_arr[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
    std::vector<int> marge_sort_vector(arr, arr + 9);
    SGI::mergesort(marge_sort_vector.begin(), marge_sort_vector.end());
    for (int i = 0; i < marge_sort_vector.size(); ++i) {
        std::cout << marge_sort_vector[i] << ' ';
    }
    std::cout << std::endl; //

    return 0;
}
