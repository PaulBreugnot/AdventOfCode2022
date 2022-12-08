add_library(day3_utils utils.cpp)

add_executable(day3_1 main_1.cpp)
target_link_libraries(day3_1 day3_utils)

add_executable(day3_2 main_2.cpp)
target_link_libraries(day3_2 day3_utils)
