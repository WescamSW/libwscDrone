cmake_minimum_required(VERSION 3.0.0)
project(wscDronepy VERSION 0.1.0)

include(CTest)
enable_testing()

# add_executable(" \
#     wscDronepy pyBebop.cpp subclass/pyBebop_Bebop2.cpp \
#     subclass/pyBebop_CameraControl.cpp subclass/pyBebop_DroneController.cpp \
#     subclass/pyBebop_DroneDiscovery.cpp subclass/pyBebop_Pilot.cpp \
#     subclass/pyBebop_Semaphore.cpp subclass/pyBebop_Utils.cpp \
#     subclass/pyBebop_VideoDecoder.cpp subclass/pyBebop_VideoDriver.cpp
#     ")
file( GLOB LIB_SOURCES lib/*.cpp )
file( GLOB LIB_HEADERS lib/*.h )
add_library( YourLib ${LIB_SOURCES} ${LIB_HEADERS} )

# If necessary, use the RELATIVE flag, otherwise each source file may be listed 
# with full pathname. RELATIVE may makes it easier to extract an executable name
# automatically.
# file( GLOB APP_SOURCES RELATIVE app/*.cxx )
file( GLOB APP_SOURCES app/*.cxx )
foreach( testsourcefile ${APP_SOURCES} )
    # I used a simple string replace, to cut off .cpp.
    string( REPLACE ".cpp" "" testname ${testsourcefile} )
    add_executable( ${testname} ${testsourcefile} )
    # Make sure YourLib is linked to each app
    target_link_libraries( ${testname} YourLib )
endforeach( testsourcefile ${APP_SOURCES} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
include(CPack)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

list(APPEND CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(tutorial)
  # Find default python libraries and interpreter
  find_package(PythonInterp REQUIRED)
  find_package(PythonLibs REQUIRED)
  include(BuildBoost) # Custom module

  include_directories(${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIR})

  # Build and link the pylib module
  add_library(pylib SHARED pylib.cpp)
  target_link_libraries(pylib ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
  add_dependencies(mylib Boost)

  # Tweaks the name of the library to match what Python expects
  set_target_properties(pylib PROPERTIES SUFFIX .so)
  set_target_properties(pylib PROPERTIES PREFIX "")