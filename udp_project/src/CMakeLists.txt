cmake_minimum_required(VERSION 3.0.0)
project(udp_server VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GTEST_DIR "/usr/src/googletest")


include(CTest)
enable_testing()

find_package(GTest REQUIRED)


file(GLOB_RECURSE HEADER "include/*.h")
file(GLOB_RECURSE SOURCE  "src/*.cpp" "*.cpp" "../test/*.cpp")


add_executable(${PROJECT_NAME} ${SOURCE} ${HEADER})


target_link_libraries(${PROJECT_NAME} gtest gtest_main pthread)

source_group("Headers" FILES ${HEADER})
source_group("Sources" FILES ${SOURCE})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


include(CPack)

set(CMAKE_MODULE_PATH /usr/include/gmock)
