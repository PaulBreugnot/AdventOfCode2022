add_library(day5_utils utils.cpp)

add_executable(day5_1 main_1.cpp)
target_link_libraries(day5_1 day5_utils)

add_executable(day5_2 main_2.cpp)
target_link_libraries(day5_2 day5_utils)
