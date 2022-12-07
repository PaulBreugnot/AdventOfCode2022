add_library(day2 score.cpp)

add_executable(day2_1 main_1.cpp)
add_executable(day2_2 main_2.cpp)

target_link_libraries(day2_1 day2)
target_link_libraries(day2_2 day2)
