add_library(day12_utils utils.cpp)

add_executable(day12_1 main_1.cpp)
target_link_libraries(day12_1 day12_utils)

add_executable(day12_2 main_2.cpp)
target_link_libraries(day12_2 day12_utils)
