cmake_minimum_required(VERSION 3.23)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(test main.cpp menu.h menu.cpp user.h user.cpp reader.h reader.cpp library.h library.cpp bookcm.cpp bookcm.h)
