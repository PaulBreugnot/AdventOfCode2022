add_library(day18_utils utils.cpp)

add_executable(day18_1 main_1.cpp)
target_link_libraries(day18_1 day18_utils)

add_executable(day18_2 main_2.cpp)
target_link_libraries(day18_2 day18_utils)

add_executable(
  day18_test
  test.cpp
)
target_link_libraries(
  day18_test
  day18_utils
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(day18_test)

