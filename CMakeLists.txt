cmake_minimum_required(VERSION 3.5)

project(OsgAquarium LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Check if we are doing out of source builds
string(COMPARE NOTEQUAL ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} CMAKE_OUT_OF_SOURCE_BUILD)

# Relative paths can be trouble when doing out of source builds
if(CMAKE_OUT_OF_SOURCE_BUILD)
  set(DEFAULT_RELATIVE_PATHS OFF)
else()
  set(DEFAULT_RELATIVE_PATHS ON)
endif()

# Show relative paths options
option(CMAKE_USE_RELATIVE_PATHS "If true, cmake will use relative paths in makefiles and projects." ${DEFAULT_RELATIVE_PATHS})

if(CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_CONFIGURATION_TYPES Debug Release)
   set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
     "Reset the configurations to what we need"
     FORCE)
endif()


if(WIN32)
  # Where to find OSG directory
  set(OSG_DIR $ENV{OSG_DIR} CACHE PATH "Path where to find the OpenSceneGraph")
  
  # Where to find OSG 3rd party directory
  set(OSG_THIRD_PARTY_DIR CACHE PATH "Path where to find the osg third party dependencies")
  
  # Where to find OpenCV3 directory
  set(OPENCV_ROOT_DIR CACHE PATH "Path where to find the OpenCV")
endif()

#######################################
# Library files
#######################################
find_package( OpenGL REQUIRED )
find_package( OpenSceneGraph REQUIRED osgViewer osgDB osgGA)
find_package( OpenCV REQUIRED )


# Copy textures if doing an out of source build
if(CMAKE_OUT_OF_SOURCE_BUILD)
  configure_file(textures/grid.png textures/grid.png COPYONLY)
  configure_file(textures/fish1.png textures/fish1.png COPYONLY)
  configure_file(textures/fish2.png textures/fish2.png COPYONLY)
  configure_file(textures/fish3.png textures/fish3.png COPYONLY)
  
  configure_file(haarcascades/haarcascade_frontalface_alt.xml haarcascades/haarcascade_frontalface_alt.xml COPYONLY)
endif()

###############################################################################
# Compile subdirectory
################################################################################
add_subdirectory(src)

