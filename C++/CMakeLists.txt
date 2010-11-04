cmake_minimum_required (VERSION 2.6)

project(ContactAngle)
set(ContactAngle_VERSION_MAJOR 0)
set(ContactAngle_VERSION_MINOR 1)

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

set(wxWidgets_USE_LIBS base core)

find_package(wxWidgets REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

include(${wxWidgets_USE_FILE})

include_directories(\
  ${ContactAngle_SOURCE_DIR} \
  ${wxWidgets_INCLUDE_DIRS} \
  ${OpenCV_INCLUDE_DIRS})

add_executable(ContactAngle main.cpp MainFrame.h MainFrame.cpp fit.h fit.cpp)
target_link_libraries(ContactAngle ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS ContactAngle DESTINATION bin)
