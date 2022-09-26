Cmake_minimum_required (VERSION 3.0)


project(mercure_driver)

message(STATUS "Configuring Sub_project:mercure_drive")

find_package(OpenCV 4 REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENCV_INCLUDE_DIRECTORIES}
)

#detect_factory

add_library(mercure_driver mercure_driver.cpp)

target_link_libraries(mercure_driver
    ${OpenCV_LIBRARIES}
	gxiapi
)


