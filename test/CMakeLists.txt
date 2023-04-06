cmake_minimum_required(VERSION 3.14)

project(tinjaTests LANGUAGES CXX)

#find_package(Catch2 3 REQUIRED)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.2 # or a later release
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
  Bustache
  GIT_REPOSITORY https://github.com/jamboree/bustache.git
  GIT_TAG ba3f0e7b295ce785f13aacf3ab6283b33c4aeac6
)
FetchContent_MakeAvailable(Bustache)

add_executable(tinja_tests
  source/benchmark.cpp
  source/test.cpp
)
target_include_directories(tinja_tests
PRIVATE
  ../include
  third_party
)
target_link_libraries(tinja_tests
PRIVATE
  Catch2::Catch2WithMain
  bustache
)
target_compile_features(tinja_tests
PRIVATE
  cxx_std_17
)
set_property(TARGET tinja_tests PROPERTY CXX_STANDARD 17)

configure_file(data/circuco_basic.html ${CMAKE_CURRENT_BINARY_DIR}/circuco_basic.html COPYONLY)
configure_file(data/circuco_inja.html ${CMAKE_CURRENT_BINARY_DIR}/circuco_inja.html COPYONLY)
configure_file(data/circuco_mustache.html ${CMAKE_CURRENT_BINARY_DIR}/circuco_mustache.html COPYONLY)
configure_file(data/circuco_tinja.html ${CMAKE_CURRENT_BINARY_DIR}/circuco_tinja.html COPYONLY)

include(CTest)
#include(Catch)
#catch_discover_tests(tinja_tests)

# ---- from cmake-init ----
enable_testing()
add_test(NAME tinja_tests COMMAND tinja_tests)
