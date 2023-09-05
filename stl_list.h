#include "stl_iterator_base.h"
#include "stl_construct.h"

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

    template <class T, class Alloc = alloc> // 缺省使用 alloc 为配置器
    class list {
    protected:
        typedef __list_node<T> list_node;
        // 专属空间配置器，每次配置一个节点大小
        typedef simple_alloc<list_node, Alloc> list_node_allocator;

    public:
        typedef list_node* lint_type;

        list() { empty_initialize(); } // 产生一个空链表

        // 让node指针，刻意置于尾端的一个空白节点，node 就符合 STL 前闭后开区间的要求
        iterator begin() { return (lint_type)((*node).next); }
        iterator end() { return node; }
        bool empty() const { return node->next == node; }
        size_type size() const {
            size_type result = 0;
            distance(begin(), end(), result);
            return result;
        }
        // 取头节点内容（元素值）
        reference front() { return *begin(); }
        // 取尾节点内容（元素值）
        reference back() { return *(--end()); }

    protected:
        link_type node; // 只需要一个指针，便可以表示整个环状双向链表
        // 配置、释放、构造、消耗一个节点
        // 配置一个节点并传回
        link_type get_node() { return list_node_allocator::allocate(); }
        // 释放一个节点
        void put_node(link_type p) { list_node_allocator::deallocate(p); }
        // 产生（配置并构造）一个节点，带有元素值
        link_type create_node(const T& x) {
            link_type p = get_node();
            construct(&p->data, x); // 全局函数，构造/析构基本工具
            return p;
        }
        // 销毁（析构并释放）一个节点
        void destroy_node(link_type p) {
            destroy(&p->data); // 全局函数，构造/ 析构基本工具
            put_node(p);
        }

        void empty_initalize() {
            node = get_node(); // 配置一个节点空间，令 node 指向他
            node->next = node; // 令 node 头尾都指向自己，不设置元素
            node->prev = node;
        }

        void push_back(const T& x) {
            insert(end(), x);
        }
        // 首先配置并构造一个节点，在尾端进行适当指针操作，将新节点插入
        iterator insert(iterator position, const T& x) {
            link_type tmp = create_node(x); // 产生一个新节点，设置内容为x
            // 调整双指针，使 tmp 插入
            tmp->next = position.node;
            tmp->prev = position.node->prev;
            (lint_type(position.node->prev))->next = tmp;
            position.node->prev = tmp;
            return tmp;
        }
    };


#endif //LEARNING_STL_LIST_H
}