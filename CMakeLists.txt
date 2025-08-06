#Auto Generated Root CMake file by Sage
#Copyright(c) 2025 None.All rights reerved.
cmake_minimum_required(VERSION 3.6...3.31)
project(BitcoinExRC VERSION 0.1.0 LANGUAGES CXX C)
include(cmake/config.cmake)
#@add_find_package Warning: Do not remove this line
find_package(nlohmann_json)
find_package(CURL)

#@add_subproject Warning: Do not remove this line
add_subdirectory(BitcoinExRC)