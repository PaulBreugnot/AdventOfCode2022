add_library(day15_utils utils.cpp)

add_executable(day15_1 main_1.cpp)
target_link_libraries(day15_1 day15_utils)

add_executable(day15_2 main_2.cpp)
target_link_libraries(day15_2 day15_utils)

add_executable(
  day15_1_test
  test_1.cpp
)
target_link_libraries(
  day15_1_test
  day15_utils
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(day15_1_test)

