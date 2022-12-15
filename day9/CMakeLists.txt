add_library(day9_utils utils.cpp)

add_executable(day9_1 main_1.cpp)
target_link_libraries(day9_1 day9_utils)

add_executable(day9_2 main_2.cpp)
target_link_libraries(day9_2 day9_utils)

