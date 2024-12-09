cmake_minimum_required(VERSION 3.20)

project(advent_of_code_2024)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wsign-conversion -pedantic")

add_executable(day_2024_01 day_2024_01.cpp shared.cpp)
add_executable(day_2024_02 day_2024_02.cpp shared.cpp)
add_executable(day_2024_03 day_2024_03.cpp shared.cpp)
add_executable(day_2024_04 day_2024_04.cpp shared.cpp)
add_executable(day_2024_05 day_2024_05.cpp shared.cpp)
add_executable(day_2024_06 day_2024_06.cpp shared.cpp)
add_executable(day_2024_07 day_2024_07.cpp shared.cpp)
add_executable(day_2024_08 day_2024_08.cpp shared.cpp)
add_executable(aoc_2024_09 aoc_2024_09.cpp shared.cpp)
