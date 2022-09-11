cmake_minimum_required(VERSION 3.16.3)
project(RM_vison)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPATH=\"\\\"${PROJECT_SOURCE_DIR}\\\"\"")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${CMAKE_SYSTEM_NAME}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")


FIND_PACKAGE(OpenCV 4 REQUIRED)
FIND_PACKAGE(Threads)
FIND_PACKAGE(Eigen3 REQUIRED)

INCLUDE_DIRECTORIES(
        "include/"
        "include/Armor"
        "include/Classifier"
        "include/Const"
        "include/Debug"
        "include/Extend"
        "include/ImageProgress"
        "include/Serial"
        "include/Solver"
        ${OPENCV_INCLUDE_DIRECTORIES}
        ${Eigen_INCLUDE_DIRECTORIES}
)

add_executable(RM_vison main.cpp include/Armor/ArmorBox.h include/Armor/ArmorDetector.h include/Armor/Classfier.h include/Solver/Solver.h include/Serial/CanSerial.h include/Serial/Serial.h include/Extend/Tools.h include/Extend/Mat_time.h include/ImageProgress/ImageProgess.h include/Debug/Debug.h include/Debug/DebugParam.h src/Armor/ArmorBox.cpp src/Armor/ArmorDetector.cpp src/Armor/Classfier.cpp src/Debug/Debug.cpp src/Extend/Mat_time.cpp src/Extend/Tools.cpp src/ImageProgress/ImageProgress.cpp src/Serial/CanSerial.cpp src/Serial/Serial.cpp src/Solver/Solver.cpp include/Classifier/C_Classifier.h include/Classifier/Opencv_Classifier.h src/Classifier/C_Classifier.cpp src/Classifier/Opencv_Classifier.cpp)

TARGET_LINK_LIBRARIES(RM_vison  ${OpenCV_LIBS})
