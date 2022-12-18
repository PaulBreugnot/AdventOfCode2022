add_library(day13_utils utils.cpp)

add_executable(day13_1 main_1.cpp)
target_link_libraries(day13_1 day13_utils)

add_executable(day13_2 main_2.cpp)
target_link_libraries(day13_2 day13_utils)

enable_testing()

add_executable(
  day13_1_test
  test_1.cpp
)
target_link_libraries(
  day13_1_test
  day13_utils
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(day13_1_test)

