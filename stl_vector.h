#include "stl_construct.h"
#ifndef LEARNING_STL_STL_VECTOR_H
#define LEARNING_STL_STL_VECTOR_H

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
    if (finish != end_of_storage) { // 还有备用空间
        // 很奇怪的逻辑，与push_back重复了。而且二者实现不一样。这是什么考虑呢？

        // 在备用空间起始处，构造一个元素，并以 vector 最后一个元素值为其初值
        construct(finish, *(finish - 1));
        ++finish; // 调整水位
        T x_copy = x;
        copy_backward(position, finish - 2, finish - 1)
        *position = x_copy;
    }
    else { // 已无备用空间
        const size_type old_size = size();
        // 如果原来大小为0，则配置1个元素大小
        // 如果原大小不为0， 则配置原来大小的两倍
        // 前半段用来放置原数据，后半段准备用来放置新数据
        const size_type len = old_size != 0 ? 2 * old_size : 1;

        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        try {
            // 将原vector 内容复制到新 vector
            new_finish = uninitialized_copy(start, position, new_start);
            // 新元素设定初值x
            construct(new_finish, x);
            // 调整水位
            ++new_finish;
            // 将原vector 的备用空间中内容也忠实复制过来（侯捷疑惑：啥用途？）
            new_finish = uninitialized_copy(position, finish, new_finish);
        }
        catch(...) {
            destory(new_start, new_finish);
            data_allocator::deallocator(new_start, len);
            throw;
        }

        // 析构并释放原来的 vector
        destory(begin(), end());
        deallocate();

        // 调整迭代器，指向新 vector
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    }
}

void push_back(const T& x) {
    if (finish != end_of_storage) { // 还有备用空间
        construct(finish, x);
        ++finish; //  调整水位高度
    }
    else // 无备用空间
        insert_aux(end(), x);
}

#endif //LEARNING_STL_STL_VECTOR_H
