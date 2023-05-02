project(tests)

add_executable(${PROJECT_NAME} "test_set.cpp")
add_executable(${PROJECT_NAME}_external "test_external.cpp")

target_link_libraries(${PROJECT_NAME} task_5_lib GTest::gtest_main)
target_link_libraries(${PROJECT_NAME}_external task_5_lib)
gtest_discover_tests(${PROJECT_NAME})
