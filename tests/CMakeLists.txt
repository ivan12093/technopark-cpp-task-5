project(tests)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME} GTest::gtest_main)
gtest_discover_tests(${PROJECT_NAME})

if (BUILD_COVERAGE)
    add_coverage(${PROJECT_NAME})
endif()
