add_library(day7_utils utils.cpp)

add_executable(day7_1 main_1.cpp)
target_link_libraries(day7_1 day7_utils)

add_executable(day7_2 main_2.cpp)
target_link_libraries(day7_2 day7_utils)
