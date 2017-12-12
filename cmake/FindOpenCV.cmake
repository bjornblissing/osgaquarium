# - Find OpenCV SDK
# Find the native OpenCV headers and libraries.
#
#  OPENCV_INCLUDE_DIRS - where to find header files
#  OPENCV_LIBRARIES    - List of libraries when using OpenCV.
#  OPENCV_BIN_DIR      - Path to binaries needed for using OpenCV, i.e. DLL-files
#  OPENCV_FOUND        - True if OpenCV found.


set(OPENCV_ROOT_DIR
    "${OPENCV_ROOT_DIR}"
    CACHE
    PATH
    "Root directory to search for OpenCV")

# Look for the header file.
find_path(OPENCV_INCLUDE_DIRS NAMES opencv2/opencv.hpp HINTS 
  ${OPENCV_ROOT_DIR}/include/
)

  
# Try to ascertain the version of the SDK
if(OPENCV_INCLUDE_DIRS)
  set(_OPENCV_VERSION_FILE "${OPENCV_INCLUDE_DIRS}/opencv2/core/version.hpp")
  if(EXISTS "${_OPENCV_VERSION_FILE}") 
    file(STRINGS "${_OPENCV_VERSION_FILE}" _OPENCV_VERSION_FILE_CONTENTS REGEX "#define CV_VERSION_[A-Z]+[ \t]+[0-9]+")
    string(REGEX REPLACE ".*#define CV_VERSION_MAJOR[ \t]+([0-9]+).*" "\\1" OPENCV_VERSION_MAJOR ${_OPENCV_VERSION_FILE_CONTENTS})
    string(REGEX REPLACE ".*#define CV_VERSION_MINOR[ \t]+([0-9]+).*" "\\1" OPENCV_VERSION_MINOR ${_OPENCV_VERSION_FILE_CONTENTS})
    string(REGEX REPLACE ".*#define CV_VERSION_REVISION[ \t]+([0-9]+).*" "\\1" OPENCV_VERSION_REVISION ${_OPENCV_VERSION_FILE_CONTENTS})
    
    set(OPENCV_VERSION "${OPENCV_VERSION_MAJOR}.${OPENCV_VERSION_MINOR}.${OPENCV_VERSION_REVISION}" CACHE INTERNAL "The version of OpenCV which was detected")
    set(_OPENCV_VERSION "${OPENCV_VERSION_MAJOR}${OPENCV_VERSION_MINOR}${OPENCV_VERSION_REVISION}" CACHE INTERNAL "The short version of the OpenCV version number")
  endif()
endif()

# Determine architecture
if(CMAKE_SIZEOF_VOID_P MATCHES "8")
  if(MSVC)
    set(_OPENCV_LIB_ARCH "x64")
  endif()
else()
  if(MSVC)
    set(_OPENCV_LIB_ARCH "x86")
  endif()
endif()


# Determine the compiler version for Visual Studio
if(MSVC)
  # Visual Studio 2010
  if(${MSVC_VERSION} EQUAL 1600)
    set(_OPENCV_MSVC_DIR "vc10")
  endif()
  # Visual Studio 2012
  if(${MSVC_VERSION} EQUAL 1700)
    set(_OPENCV_MSVC_DIR "vc11")
  endif()
  # Visual Studio 2013
  if(${MSVC_VERSION} EQUAL 1800)
    set(_OPENCV_MSVC_DIR "vc12")
  endif()
  # Visual Studio 2015
  if(${MSVC_VERSION} EQUAL 1900)
    set(_OPENCV_MSVC_DIR "vc14")
  endif()
    # Visual Studio 2017
  if(MSVC_VERSION EQUAL 1910 OR MSVC_VERSION EQUAL 1911)
    set(_OPENCV_MSVC_DIR "vc15")
  endif()
endif()

# Append "d" to debug libs on windows platform
if(WIN32)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(_OPENCV_LIB_DIR ${OPENCV_ROOT_DIR}/${_OPENCV_LIB_ARCH}/${_OPENCV_MSVC_DIR}/lib)

# Look for the library
find_library(OPENCV_CORE_LIBRARY NAMES opencv_core${_OPENCV_VERSION} HINTS ${OPENCV_ROOT_DIR} ${_OPENCV_LIB_DIR})
find_library(OPENCV_CORE_LIBRARY_DEBUG NAMES opencv_core${_OPENCV_VERSION}${CMAKE_DEBUG_POSTFIX} HINTS ${OPENCV_ROOT_DIR} ${_OPENCV_LIB_DIR})

find_library(OPENCV_VIDEOIO_LIBRARY NAMES opencv_videoio${_OPENCV_VERSION} HINTS ${OPENCV_ROOT_DIR} ${_OPENCV_LIB_DIR})
find_library(OPENCV_VIDEOIO_LIBRARY_DEBUG NAMES opencv_videoio${_OPENCV_VERSION}${CMAKE_DEBUG_POSTFIX} HINTS ${OPENCV_ROOT_DIR} ${_OPENCV_LIB_DIR})

find_library(OPENCV_IMGPROC_LIBRARY NAMES opencv_imgproc${_OPENCV_VERSION} HINTS ${OPENCV_ROOT_DIR} ${_OPENCV_LIB_DIR})
find_library(OPENCV_IMGPROC_LIBRARY_DEBUG NAMES opencv_imgproc${_OPENCV_VERSION}${CMAKE_DEBUG_POSTFIX} HINTS ${OPENCV_ROOT_DIR} ${_OPENCV_LIB_DIR})

find_library(OPENCV_OBJDETECT_LIBRARY NAMES opencv_objdetect${_OPENCV_VERSION} HINTS ${OPENCV_ROOT_DIR} ${_OPENCV_LIB_DIR})
find_library(OPENCV_OBJDETECT_LIBRARY_DEBUG NAMES opencv_objdetect${_OPENCV_VERSION}${CMAKE_DEBUG_POSTFIX} HINTS ${OPENCV_ROOT_DIR} ${_OPENCV_LIB_DIR})


set(OPENCV_LIBRARY ${_OPENCV_CORE_LIBRARY} ${_OPENCV_VIDEOIO_LIBRARY} ${_OPENCV_IMGPROC_LIBRARY})
set(OPENCV_LIBRARY_DEBUG ${_OPENCV_CORE_LIBRARY_DEBUG} ${_OPENCV_VIDEOIO_LIBRARY_DEBUG} ${_OPENCV_IMGPROC_LIBRARY_DEBUG})

set(OPENCV_LIBRARIES 
  optimized 
    ${OPENCV_CORE_LIBRARY}
    ${OPENCV_VIDEOIO_LIBRARY} 
    ${OPENCV_IMGPROC_LIBRARY}
    ${OPENCV_OBJDETECT_LIBRARY}
    
  debug
    ${OPENCV_CORE_LIBRARY_DEBUG}
    ${OPENCV_VIDEOIO_LIBRARY_DEBUG}
    ${OPENCV_IMGPROC_LIBRARY_DEBUG}
    ${OPENCV_OBJDETECT_LIBRARY_DEBUG}
    
)
# Find binaries i.e. DLL path
if(MSVC)
  FIND_PATH(OPENCV_BIN_DIR opencv_core${_OPENCV_VERSION}.dll HINTS ${OPENCV_ROOT_DIR} ${OPENCV_ROOT_DIR}/${_OPENCV_LIB_ARCH}/${_OPENCV_MSVC_DIR}/bin)
  MARK_AS_ADVANCED(OPENCV_BIN_DIR)
endif()

# handle the QUIETLY and REQUIRED arguments and set OPENCV_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenCV DEFAULT_MSG OPENCV_LIBRARIES OPENCV_INCLUDE_DIRS)

mark_as_advanced(OPENCV_LIBRARIES OPENCV_INCLUDE_DIRS)
