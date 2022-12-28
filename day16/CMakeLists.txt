add_library(day16_utils utils.cpp)

add_executable(dot_output dot.cpp)
target_link_libraries(dot_output day16_utils)

add_executable(day16_1 main_1.cpp)
target_link_libraries(day16_1 day16_utils)

add_executable(day16_2 main_2.cpp)
target_link_libraries(day16_2 day16_utils)
