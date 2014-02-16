# Copyright (c) 2010,2011 Daniel Winters <daniel@tydirium.org>
#
# Permission to use, copy, modify, and distribute this software for
# any purpose with or without fee is hereby granted, provided that the
# above copyright notice and this permission notice appear in all
# copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
# WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
# AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
# DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
# OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
# TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

cmake_minimum_required (VERSION 2.6)

project(ContactAngle)
set(ContactAngle_VERSION_MAJOR 0)
set(ContactAngle_VERSION_MINOR 2)

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

set(wxWidgets_USE_LIBS base core adv)

find_package(wxWidgets REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

include(${wxWidgets_USE_FILE})
set(wxWidgets_USE_DEBUG ON)

include_directories(${ContactAngle_SOURCE_DIR} ${wxWidgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

if(WIN32)
  add_executable(ContactAngle WIN32 main.cpp MainFrame.h MainFrame.cpp fit.h fit.cpp)
  file(GLOB dlls "${OpenCV_DIR}/bin/*.dll")
  install(FILES ${dlls} DESTINATION bin)
else()
  add_executable(ContactAngle main.cpp MainFrame.h MainFrame.cpp fit.h fit.cpp)
endif()

target_link_libraries(ContactAngle ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS ContactAngle DESTINATION bin)

file(GLOB samples "samples/*.avi")
install(FILES ${samples} DESTINATION "share/doc/ContactAngle")

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ContactAngle")
set(CPACK_PACKAGE_VENDOR "Daniel Winters <daniel@tydirium.org>")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${ContactAngle_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ContactAngle_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_EXECUTABLES "ContactAngle" "ContactAngle")

include(CPack)
