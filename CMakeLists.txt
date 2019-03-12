cmake_minimum_required(VERSION 3.12)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(compiler Data.h Data.cpp Store.h Store.cpp Scanner.h Scanner.cpp CiFa_test.cpp Grammar.h Grammar.cpp YuFa_test.cpp)

#set(SOURCE_FILES CiFa_test.cpp)
#add_executable(compiler ${SOURCE_FILES})