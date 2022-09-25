cmake_minimum_required(VERSION 3.16.3)
project(RM_vison)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPATH=\"\\\"${PROJECT_SOURCE_DIR}\\\"\"")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${CMAKE_SYSTEM_NAME}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")


FIND_PACKAGE(OpenCV 4 REQUIRED)
FIND_PACKAGE(Threads)
FIND_PACKAGE(Eigen3 REQUIRED)

INCLUDE_DIRECTORIES(
        "include/"
        "include/armor"
        "include/classifier"
        "include/const"
        "include/debug"
        "include/extend"
        "include/image_progress"
        "include/serial"
        "include/solver"
        "include/mercure"
        ${OPENCV_INCLUDE_DIRECTORIES}
        ${Eigen_INCLUDE_DIRECTORIES}
)


FILE(GLOB_RECURSE sourcefiles "src/*.cpp")
add_executable(RM_vison ${sourcefiles} include/const/const.h test_include/test.h test_src/test.cpp)


TARGET_LINK_LIBRARIES(RM_vison  ${OpenCV_LIBS})
