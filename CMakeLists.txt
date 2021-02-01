cmake_minimum_required(VERSION 3.10)

project("Smart Pointers"
  LANGUAGES CXX
)

find_package(Threads)

add_library(pointers0
  Shared.cc
)

target_compile_options(pointers0
  PRIVATE
    "-Wall" "-Wextra" "-g" "-fsanitize=address,undefined"
)

target_compile_features(pointers0
  PUBLIC
    cxx_std_17
)

set_target_properties(pointers0
  PROPERTIES
    CXX_EXTENSIONS OFF
    LINK_FLAGS "-fsanitize=address,undefined"
)

# Compile googltest as static lib
add_library(googletest1 STATIC
  googletest/googletest/src/gtest-all.cc
)

target_compile_options(googletest1
  PRIVATE
    "-g" "-O2"
)

target_include_directories(googletest1
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest"
)

add_executable(testPointers
testPointers.cc
)

target_include_directories(testPointers
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest"
)

target_link_libraries(testPointers
  PRIVATE
    pointers0
    googletest1
    Threads::Threads
)

target_compile_options(testPointers
  PRIVATE
  "-Wall" "-Wextra" "-g" "-fsanitize=address,undefined"
)

target_compile_features(testPointers
  PUBLIC
    cxx_std_17
)

set_target_properties(testPointers
  PROPERTIES
    CXX_EXTENSIONS OFF
    LINK_FLAGS "-fsanitize=address,undefined"
)
