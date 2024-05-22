cmake_minimum_required(VERSION 3.22)
project(CombLogicSim)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} main.cpp
        src/circuit.cpp
        src/gates.cpp
        src/utils.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Add the test executable
add_executable(${PROJECT_NAME}_test test.cpp
        src/circuit.cpp
        src/gates.cpp
        src/utils.cpp
#        test/test_circuit.cpp
#        test/test_gates.cpp
#        test/test_utils.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Link Google Test to the test executable
target_link_libraries(${PROJECT_NAME}_test gtest gtest_main)

target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
