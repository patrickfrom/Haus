cmake_minimum_required(VERSION 3.27)
project(HausEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(HausEngine
        Core/Application.cpp
        Core/Application.h
        HausEngine.h)