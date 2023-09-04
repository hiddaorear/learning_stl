#include <cstddef>
#include "stl_algo.h"

namespace SGI {
#ifndef LEARNING_STL_LIST_H
#define LEARNING_STL_LIST_H
    // 双向链表
    template<class T>
    struct __list_node {
        typedef void* void_pointer;
        void_pointer prev;
        void_pointer  next;
        T data;
    };

    template<class T, class Ref, class Ptr>
    struct __list_iterator {
        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, Ref, Ptr> self;

        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef __list_node<T>* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        link_type node; // 指向list的节点

        // constructor
        __list_iterator(link_type x) : node(x) {}
        __list_iterator() {}
        __list_iterator(const iterator& x) : node(x.node) {}

        bool operator==(const self& x) const { return node == x.node; }
        bool operator!=(const self& x) const { return node != x.node; }
        // 以下对迭代器取值（dereference），取的是节点的数据值
        reference  operator*() const { return (*node).data; }
        // 以下是迭代器的成员存取运算子（member access）的标准做法
        pointer operator->() const { return &(operator*()); }

        // 对迭代器累加1，就前进一个节点
        self& operator++() {
            node = (link_type)((*node).next);
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        // 对迭代器累减1，就后退一个节点
        self& operator--() {
            node = (link_type)((*node).prev);
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };

#endif //LEARNING_STL_LIST_H
}