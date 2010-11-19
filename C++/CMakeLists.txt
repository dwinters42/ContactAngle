cmake_minimum_required (VERSION 2.6)

project(ContactAngle)
set(ContactAngle_VERSION_MAJOR 0)
set(ContactAngle_VERSION_MINOR 1)

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

set(wxWidgets_USE_LIBS base core aui)

find_package(wxWidgets REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

include(${wxWidgets_USE_FILE})

include_directories(${ContactAngle_SOURCE_DIR} ${wxWidgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(ContactAngle main.cpp MainFrame.h MainFrame.cpp fit.h fit.cpp)
target_link_libraries(ContactAngle ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS ContactAngle DESTINATION bin)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ContactAngle")
set(CPACK_PACKAGE_VENDOR "Daniel Gruber <daniel@tydirium.org>")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Readme.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_EXECUTABLES "ContactAngle" "ContactAngle")

if(UNIX)
  SET(CPACK_GENERATOR "DEB")
  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Daniel Gruber") #required
endif(UNIX)

include(CPack)
