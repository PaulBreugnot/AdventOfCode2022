add_library(day11_utils utils.cpp)

add_executable(day11_1 main_1.cpp)
target_link_libraries(day11_1 day11_utils)
