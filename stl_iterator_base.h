
namespace SGI {
#ifndef LEARNING_STL_STL_ITERATOR_BASE_H
#define LEARNING_STL_STL_ITERATOR_BASE_H

#define __STL_REQUIRES(__type_var, __concept) do {} while(0)

    struct __true_type {
    };

    struct __false_type {
    };

    template<class _Tp>
    struct __type_traits {
        typedef __true_type this_dummy_member_must_be_first;
        /* Do not remove this member. It informs a compiler which
           automatically specializes __type_traits that this
           __type_traits template is special. It just makes sure that
           things work if an implementation is using a template
           called __type_traits for something unrelated. */

        /* The following restrictions should be observed for the sake of
           compilers which automatically produce type specific specializations
           of this class:
               - You may reorder the members below if you wish
               - You may remove any of the members below if you wish
               - You must not rename members without making the corresponding
                 name change in the compiler
               - Members you add will be treated like regular members unless
                 you add the appropriate support in the compiler. */


        typedef __false_type has_trivial_default_constructor;
        typedef __false_type has_trivial_copy_constructor;
        typedef __false_type has_trivial_assignment_operator;
        typedef __false_type has_trivial_destructor;
        typedef __false_type is_POD_type;
    };

# ifdef __STL_USE_EXCEPTIONS
#   define __STL_TRY try
#   define __STL_CATCH_ALL catch(...)
#   define __STL_THROW(x) throw x
#   define __STL_RETHROW throw
#   define __STL_NOTHROW throw()
#   define __STL_UNWIND(action) catch(...) { action; throw; }
# else
#   define __STL_TRY
#   define __STL_CATCH_ALL if (false)
#   define __STL_THROW(x)
#   define __STL_RETHROW
#   define __STL_NOTHROW
#   define __STL_UNWIND(action)
# endif

// value_type实现
    struct input_iterator_tag {
    };
    struct output_iterator_tag {
    };
    struct forward_iterator_tag : public input_iterator_tag {
    };
    struct bidirectional_iterator_tag : public forward_iterator_tag {
    };
    struct random_access_iterator_tag : public bidirectional_iterator_tag {
    };

    template<class _Iterator>
    struct iterator_traits {
        typedef typename _Iterator::iterator_category iterator_category;
        typedef typename _Iterator::value_type value_type;
        typedef typename _Iterator::difference_type difference_type;
        typedef typename _Iterator::pointer pointer;
        typedef typename _Iterator::reference reference;
    };

    template<class _Tp>
    struct iterator_traits<_Tp *> {
        typedef random_access_iterator_tag iterator_category;
        typedef _Tp value_type;
        typedef ptrdiff_t difference_type;
        typedef _Tp *pointer;
        typedef _Tp &reference;
    };

    template<class _Tp>
    struct iterator_traits<const _Tp *> {
        typedef random_access_iterator_tag iterator_category;
        typedef _Tp value_type;
        typedef ptrdiff_t difference_type;
        typedef const _Tp *pointer;
        typedef const _Tp &reference;
    };


    template<class _Iter>
    inline typename iterator_traits<_Iter>::iterator_category
    __iterator_category(const _Iter &) {
        typedef typename iterator_traits<_Iter>::iterator_category _Category;
        return _Category();
    }

    template<class _Iter>
    inline typename iterator_traits<_Iter>::difference_type *
    __distance_type(const _Iter &) {
        return static_cast<typename iterator_traits<_Iter>::difference_type *>(0);
    }

    template<class _Iter>
    inline typename iterator_traits<_Iter>::value_type *
    __value_type(const _Iter &) {
        return static_cast<typename iterator_traits<_Iter>::value_type *>(0);
    }

    template<class _Iter>
    inline typename iterator_traits<_Iter>::iterator_category
    iterator_category(const _Iter &__i) { return __iterator_category(__i); }


    template<class _Iter>
    inline typename iterator_traits<_Iter>::difference_type *
    distance_type(const _Iter &__i) { return __distance_type(__i); }


    template<class _Iter>
    inline typename iterator_traits<_Iter>::value_type *
    value_type(const _Iter &__i) { return __value_type(__i); }

#define __VALUE_TYPE(__i)        __value_type(__i)

// distance
// 迭代器操作函数，计算两个迭代器之间的距离

    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        // 逐一累积距离
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    template<class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator first, RandomAccessIterator last,
               random_access_iterator_tag) {
        __STL_REQUIRES(RandomAccessIterator, RandomAccessIterator);
        // 直接计算差距
        return last - first;
    }

    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
        typedef typename iterator_traits<InputIterator>::iterator_category
                _Category;
        __STL_REQUIRES(InputIterator, InputIterator);
        return __distance(first, last, _Category());
    }

#endif //LEARNING_STL_STL_ITERATOR_BASE_H
}