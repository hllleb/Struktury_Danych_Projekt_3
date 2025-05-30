cmake_minimum_required(VERSION 3.30)
project(projekt3)

set(CMAKE_CXX_STANDARD 20)

add_executable(projekt3 main.cpp
        HashTable.h
        SeparateChainingHashTable.h
        OpenAddressingHashTable.h
        CuckooHashTable.h)
