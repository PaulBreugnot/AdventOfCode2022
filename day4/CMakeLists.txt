add_library(day4_utils utils.cpp)

add_executable(day4_1 main_1.cpp)
add_executable(day4_2 main_2.cpp)

target_link_libraries(day4_1 day4_utils)
target_link_libraries(day4_2 day4_utils)
