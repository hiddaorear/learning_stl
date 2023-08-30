//
// Created by 汪怡峰 on 2023/8/24.
//

#ifndef LEARNING_STL_STL_ALGO_H
#define LEARNING_STL_STL_ALGO_H

// value_type实现
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class _Iterator>
struct iterator_traits {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type        value_type;
    typedef typename _Iterator::difference_type   difference_type;
    typedef typename _Iterator::pointer           pointer;
    typedef typename _Iterator::reference         reference;
};

template <class _Tp>
struct iterator_traits<_Tp*> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef _Tp*                        pointer;
    typedef _Tp&                        reference;
};

template <class _Tp>
struct iterator_traits<const _Tp*> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const _Tp*                  pointer;
    typedef const _Tp&                  reference;
};


template <class _Iter>
inline typename iterator_traits<_Iter>::iterator_category
__iterator_category(const _Iter&)
{
    typedef typename iterator_traits<_Iter>::iterator_category _Category;
    return _Category();
}

template <class _Iter>
inline typename iterator_traits<_Iter>::difference_type*
__distance_type(const _Iter&)
{
    return static_cast<typename iterator_traits<_Iter>::difference_type*>(0);
}

template <class _Iter>
inline typename iterator_traits<_Iter>::value_type*
__value_type(const _Iter&)
{
    return static_cast<typename iterator_traits<_Iter>::value_type*>(0);
}

template <class _Iter>
inline typename iterator_traits<_Iter>::iterator_category
iterator_category(const _Iter& __i) { return __iterator_category(__i); }


template <class _Iter>
inline typename iterator_traits<_Iter>::difference_type*
distance_type(const _Iter& __i) { return __distance_type(__i); }


template <class _Iter>
inline typename iterator_traits<_Iter>::value_type*
value_type(const _Iter& __i) { return __value_type(__i); }

#define __VALUE_TYPE(__i)        __value_type(__i)

// ----------------------------------------------------------------------------
// 数据结构

// heap

// heap push 函数
template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap(RandomAccessIterator first, Distance holdIndex,
                        Distance topIndex, T value) {
    Distance parent = (holdIndex - 1) / 2; // 找到父节点
    while (holdIndex > topIndex && *(first + parent) < value) {
        // 未到达顶端，且父节点小于新值（不符合heap次序）
        // 此处实现的为 max-heap，大者为父
        *(first + holdIndex) = *(first + parent); // 令洞值为父值
        holdIndex = parent; // percolate up，调整洞号，向上提升至父节点
        parent = (holdIndex - 1) / 2; // 新洞的父节点
    } // 持续至顶端，到满足 heap 次序为止
    *(first + holdIndex) = value; // 令洞为新值，完成插入操作
}

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first,
                            RandomAccessIterator last, Distance*, T*) {
    // 根据 implicit representation heap 的结构特性：新值必置于底部
    // 容器的最尾端，此即第一个洞号：(last - first) -1
    __push_heap(first, Distance((last - first) - 1),Distance(0),
                T(*(last - 1)));
}

template <class RandomAccessIterator>
inline void my_push_heap(RandomAccessIterator first,
                      RandomAccessIterator last) {
    // 注意，此函数调用时，新元素应已置于底部容器的最尾端
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}

// pop heap
template <class RandomAccessIterator, class Distance, class T>
inline void __adjust_heap(RandomAccessIterator first, Distance holdIndex,
                          Distance len, T value) {
    Distance topIndex = holdIndex;
    Distance secondChild = 2 * holdIndex + 2; // 洞节点之右子节点
    while (secondChild < len) {
        if (*(first + secondChild) < *(first + (secondChild - 1))) {
            // 比较洞节点之左右两个子值，然后以 secondChild 代表较大子节点
            secondChild--;
        }
        // Percolate down：令较大子值为洞值，再令洞号下移至较大子节点处
        *(first + holdIndex) = *(first + secondChild);
        holdIndex = secondChild;
        // 找出新洞节点的右子节点
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { // 没有右子节点，只有左子节点
        // Percolate down：令左子值为洞值，再令洞号下移至左子节点处
        *(first + holdIndex) = *(first + (secondChild - 1));
        holdIndex = secondChild - 1;
    }
    // 将欲调整值填入目前洞号里。注意，此时肯定满足次序特性
    // 侯捷说：下面这一句，可以改成： *(first + holdIndex) = value;
    __push_heap(first, holdIndex, topIndex, value);
}

template <class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first,
                       RandomAccessIterator last,
                       RandomAccessIterator result,
                       T value, Distance*) {
    *result = *first; // 设定尾值为首值，于是尾值为欲求之结果
    // 稍后可以再以底层容器之 pop_back() 取出尾值
    // 以上欲重新调整 heap，洞号为0（即树根处），欲调整值为 value（原尾值）
    __adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first,
                           RandomAccessIterator last, T*) {
    // 跟进 implicit representation heap 的次序，pop操作的结果
    // 应为容器底部的第一个元素。因此，首先设定欲调整值为尾值，然后将首值调至尾节点（所以以上迭代器result设为 last - 1）【更详细理解，见文章】
    // 然后重整 [first, last -1)，使之重新成为一个合格的 heap
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <class RandomAccessIterator>
inline void my_pop_heap(RandomAccessIterator first,
                     RandomAccessIterator last) {
    __pop_heap_aux(first, last, value_type(first));
}

// sort heap 堆排序
template <class RandomAccessIterator>
void my_sort_heap(RandomAccessIterator first,
               RandomAccessIterator last) {
    // 每次执行 pop_heap，极值就被放在尾端。扣除尾端，再执行pop_heap，次极值又被放到尾端。依此，可得排序结果
    while (last - first > 1) {
        pop_heap(first, last--); // 每操作一次，pop_heap()一次，操作范围退缩一格【直接修改的原来的 array，不需要新增一个array】
    }
}

// make heap
template <class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first,
                 RandomAccessIterator last, T*, Distance*) {
    if (last - first < 2) return; // 长度为 0 或 1， 不必重新排列
    Distance len = last - first;
    // 找出第一个需要重排的子树头部，以 parent 标示出，由于任何叶子节点都不需执行 Percolate down，所以有以下计算
    Distance holeIndex = (len - 2) / 2;
    while (true) {
        // 重排以 holdIndex 为首的子树。len 是为了让__adjust_heap 判断操作范围
        __adjust_heap(first, holeIndex, len, T(*(first + holeIndex)));
        if (holeIndex == 0) return; // 走完根节点，就结束
        holeIndex--; // 即将重排之子树，头部向前一个节点
    }
}

template <class RandomAccessIterator>
inline void my_make_heap(RandomAccessIterator first,
                      RandomAccessIterator last) {
    __make_heap(first, last, value_type(first), distance_type(first));
}


// ----------------------------------------------------------------------------
// 算法

// 插入排序
template <class RandomAccessIterator, class T>
void __unguarded_linear_insert(RandomAccessIterator last, T value) {
    RandomAccessIterator next = last;
    --next;
    // insertion sort 内循环
    // 注意，一旦不再出现逆转对（inversion），循环就可以结束了
    while(value < *next) { // 逆转对（inversion）存在
        *last = *next;  // 调整
        last = next;  // 调整迭代器
        --next;  // 左移一个位置
    }
    *last = value;  // value 的正确落脚处
}

template <class RandomAccessIterator, class T>
inline void __linear_insert(RandomAccessIterator first,
                            RandomAccessIterator last, T*) {
    T value = *last;  // 记录尾元素
    if (value < *first) { // 尾比头还小（注意，头端必为最小元素）
        // 比最小元素还小，不需要一个个比较，直接一次处理
        std::copy_backward(first, last, last + 1);  // 将整个区间向右移一个位置
        *first = value;  // 令头元素等于原先的尾元素值
    }
    else  // 尾不小于头
        __unguarded_linear_insert(last, value);
}

// 插入排序
template <class RandomAccessIterator>
void __insertion_sort(RandomAccessIterator first,
                      RandomAccessIterator last) {
    if (first == last) return;
    for (RandomAccessIterator i = first + 1; i != last; ++i)  // 外循环
        __linear_insert(first, i, __VALUE_TYPE(first)); // 以上，[first, i) 形成一个子区间
}

// 快排
const int __stl_threshold = 16;

// 返回a，b，c之居中者
template<class T>
inline const T& __median(const T& a, const T& b, const T& c) {
    if (a < b) {
        if (b < c) { // a < b < c
            return b;
        } else if (a < c) { // a < c < b
            return c;
        } else { // c < a < b
            return a;
        }
    } else if ( a < c) { // b < a < c
        return a;
    } else if ( b < c) { // b < c < a
        return c;
    } else { // c < b < a
        return b;
    }
}

// 分割函数，返回值为分割以后，右段第一个位置
template <class RandomAccessIterator, class T>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first,
                                           RandomAccessIterator last,
                                           T pivot) {
    while (true) {
        while (*first < pivot) ++first; // first 找到 >= pivot 的元素就停下来
        --last;
        while(pivot < *last) --last; // last 找到 <= pivot 的元素就停下来
        if (!(first < last)) {
            return first; // 交错，结束循环
        }
        std::iter_swap(first, last); // 大小值，交换
        ++first;
    }
}

template <class RandomAccessIterator>
inline void __quick_sort_loop(RandomAccessIterator first,
                              RandomAccessIterator last);


template <class RandomAccessIterator, class T>
inline void __quick_sort_loop_aux(RandomAccessIterator first,
                                  RandomAccessIterator last,
                                  T*) {
    while (last - first > __stl_threshold) {
        // median-of-3 partitioning
        RandomAccessIterator cut = __unguarded_partition(first, last,
                                                         T(__median(*first, *(first + (last - first)/2),
                                                                    *(last - 1))));
        if (cut - first >= last - cut) {
            __quick_sort_loop(cut, last); // 对右段递归处理
            last = cut;
        } else {
            __quick_sort_loop(first, cut); // 对左段递归处理
            first = cut;
        }
    }
}

template <class RandomAccessIterator>
inline void __quick_sort_loop(RandomAccessIterator first,
                              RandomAccessIterator last) {
    __quick_sort_loop_aux(first, last, __VALUE_TYPE(first));
}


// 具体实现，见上文的插入排序
template <class RandomAccessIterator>
inline void __final_insertion_sort(RandomAccessIterator first,
                                   RandomAccessIterator last) {
    __insertion_sort(first, last);
}


template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first,
                 RandomAccessIterator last) {
    if (!(first == last)) {
        __quick_sort_loop(first, last);
        __final_insertion_sort(first, last);
    }
}

#endif //LEARNING_STL_STL_ALGO_H
