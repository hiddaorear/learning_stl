cmake_minimum_required(VERSION 3.24)
project(learning_stl)

set(CMAKE_CXX_STANDARD 23)

add_executable(learning_stl main.cpp stl_algo.h stl_list.h stl_iterator_base.h stl_pair.h stl_tempbuf.h stl_uninitialized.h stl_construct.h)
