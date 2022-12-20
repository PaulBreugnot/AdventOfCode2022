add_library(day14_utils utils.cpp)

add_executable(day14_1 main_1.cpp)
target_link_libraries(day14_1 day14_utils)
