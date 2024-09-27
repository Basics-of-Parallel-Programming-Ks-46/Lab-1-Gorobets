add_library(lib lib.cpp)
add_executable(main main.cpp)
target_link_libraries(main PUBLIC lib)
