add_executable(tests tests.cpp)
target_link_libraries(tests PUBLIC lib)

include(CTest)
add_test(NAME todo COMMAND tests todo)

