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
set(wxWidgets_USE_DEBUG ON)

include_directories(${ContactAngle_SOURCE_DIR} ${wxWidgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(ContactAngle main.cpp MainFrame.h MainFrame.cpp fit.h fit.cpp)
target_link_libraries(ContactAngle ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS ContactAngle DESTINATION bin)

FILE(GLOB dlls "${OpenCV_DIR}/bin/*.dll")
install(FILES ${dlls} DESTINATION bin)
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ContactAngle")
set(CPACK_PACKAGE_VENDOR "Daniel Gruber <daniel@tydirium.org>")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Readme.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_EXECUTABLES "ContactAngle" "ContactAngle")

include(CPack)
