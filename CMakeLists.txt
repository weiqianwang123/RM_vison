cmake_minimum_required(VERSION 3.0)
project(RM_vison)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE Debug)
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
        "include/communication"
        "include/solver"
        "include/mercure"
        ${OPENCV_INCLUDE_DIRECTORIES}
        ${Eigen_INCLUDE_DIRECTORIES}
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/src/mercure
)
FILE(GLOB_RECURSE sourcefiles "src/*.cpp")
add_executable(RM_vison   ${sourcefiles} main.cpp include/solver/predict.h src/solver/predict.cpp include/communication/base.h include/communication/serial_communication.h src/communication/serial_communication.cpp include/communication/serial_datacheck.h src/communication/serial_datacheck.cpp)


TARGET_LINK_LIBRARIES(RM_vison mercure_driver ${OpenCV_LIBS})


