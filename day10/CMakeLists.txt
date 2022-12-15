add_library(day10_utils utils.cpp)

add_executable(day10_1 main_1.cpp)
target_link_libraries(day10_1 day10_utils)

add_executable(day10_2 main_2.cpp)
target_link_libraries(day10_2 day10_utils)
