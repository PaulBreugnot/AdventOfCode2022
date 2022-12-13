add_library(day8_utils utils.cpp)

add_executable(day8_1 main_1.cpp)
target_link_libraries(day8_1 day8_utils)

add_executable(day8_2 main_2.cpp)
target_link_libraries(day8_2 day8_utils)
