add_library(day17_utils utils.cpp)

add_executable(day17_1 main_1.cpp)
target_link_libraries(day17_1 day17_utils)
