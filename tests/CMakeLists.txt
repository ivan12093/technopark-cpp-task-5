project(tests)

add_executable(${PROJECT_NAME} "test_set.cpp")
add_executable(${PROJECT_NAME}_external "test_external.cpp")

target_link_libraries(${PROJECT_NAME} GTest::gtest_main)
gtest_discover_tests(${PROJECT_NAME})
