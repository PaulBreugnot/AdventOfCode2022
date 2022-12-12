add_library(day6_utils utils.cpp)

add_executable(day6_1 main_1.cpp)
target_link_libraries(day6_1 day6_utils)

add_executable(day6_2 main_2.cpp)
target_link_libraries(day6_2 day6_utils)
