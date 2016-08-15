project(fixed_point)

# CMake 2.8.11 is required by Google Benchmarks
cmake_minimum_required(VERSION 2.8.11)

add_library(
	fixed_point STATIC 
	include/sg14/fixed_point.h
	include/sg14/type_traits.h
	include/sg14/auxiliary/elastic.h
	include/sg14/auxiliary/elastic_integer.h
	include/sg14/auxiliary/integer.h
	include/sg14/auxiliary/multiprecision.h
)
set_target_properties(fixed_point PROPERTIES LINKER_LANGUAGE CXX)
include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR})
